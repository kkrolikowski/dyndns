<?php
  session_start();
  include_once './inc/config.php';
  include 'functions.php';

  $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
  $dbh = new PDO($dsn, LOGIN, PASS);
  $tool = new Toolkit;

  if(isset($_SESSION['userlogin'])) {
    $q = $dbh->prepare("SELECT role FROM users WHERE login = '".$_SESSION['userlogin']."'");
    $q->execute();
    $res = $q->fetch();
    if($res['role'] == 'admin') {
      if(isset($_GET['domid'])) {
        $q = $dbh->prepare("SELECT id,ttl,domain,owner,serial,admin_contact,master_dns FROM domains WHERE id = ".$_GET['domid']);
        $q->execute();
        $res = $q->fetch();
        $q2 = $dbh->prepare("SELECT id,subdomain,ip,type FROM subdomains WHERE domain_id = ".$_GET['domid']);
        $q2->execute();
        while($res2 = $q2->fetch()) {
          $sub[$res2['subdomain']][] = array('subid' => $res2['id'], 'ip' => $res2['ip'], 'type' => $res2['type']);
        }
        $json = array(
          'id' => $res['id'],
          'owner' => $res['owner'],
          'serial' => $res['serial'],
          'ttl' => $res['ttl'],
          'hostmaster' => $res['admin_contact'],
          'masterdns' => $res['master_dns'],
          'origin' => $res['domain'],
          'records' => $sub
        );
        header('Content-Type: application/json');
        echo json_encode($json);
      }
      if($_SERVER['REQUEST_METHOD'] == 'PUT') {
          parse_str(file_get_contents("php://input"), $json);
          $serial = $tool->calculateSerial($json['origin']);

          $q = $dbh->prepare(
          "UPDATE domains SET ttl = ".$json['ttl']. ", serial = ".$serial.", admin_contact = '".$json['hostmaster']."', " .
          "master_dns = '".$json['masterdns']."' WHERE id = ".$json['id']);
          $q->execute();

          foreach ($json['subid'] as $id => $val) {
            $q = $dbh->prepare("UPDATE subdomains SET subdomain = '".$val[0]."', type = '".$val[1]."', ip = '".$val[2]."' WHERE id = ".$id);
            $q->execute();
          }
          //print_r($json);
      }
    }
    if(isset($_GET['rm'])) {
      // resolve domain name
      $q = $dbh->prepare("SELECT domain FROM domains WHERE id = (SELECT domain_id FROM subdomains WHERE subdomains.id = ".$_GET['rm'].")");
      $q->execute();
      $res = $q->fetch();
      $domain = substr($res['domain'], 0, -1);

      // prepare serial
      $serial = $tool->calculateSerial($domain);

      // delete subdomain
      $q = $dbh->prepare("DELETE FROM subdomains WHERE id = ".$_GET['rm']);
      $q->execute();

      // update serial
      $q = $dbh->prepare("UPDATE domains SET serial = ".$serial." WHERE domain = '".$domain.".'");
      $q->execute();
    }
    /*
     * Delete domain
    */
    if(isset($_GET['rmdomain'])) {
      // verify if domain is in use
      $q = $dbh->prepare("SELECT subdomain FROM subdomains WHERE domain_id = (SELECT id FROM domains WHERE domain = '".$_GET['rmdomain'].".') AND dynamic = 1");
      $q->execute();
      if($q->rowCount() > 0) {
        header('X-Message: Domain is still in use! Check your profile', true, 406);
      }
      else {
        $q = $dbh->prepare("UPDATE domains SET domainstatus = 'delete' WHERE domain = '".$_GET['rmdomain'].".'");
        $q->execute();
      }
    }
    /*
     * Adding new subdomain to an existing domain
    */
    if(isset($_GET['subd'])) {
    	// obtain domain serial value from database
    	$actual_serial = $tool->calculateSerial($_POST['domain']);

      // obtain client IP Address
      $clientip = $tool->clientIP();

      // add subdomain record
    	$q = $dbh->prepare(
    	"INSERT INTO subdomains(user_id,domain_id,subdomain,ip,type) VALUES(".
    	"(SELECT id FROM users WHERE login = '".$_SESSION['userlogin']."'), ".
    	"(SELECT id FROM domains WHERE domain = '".$_POST['domain'].".'), '".
    	$_GET['subd']. "', '".$clientip."', 'A')");
    	$q->execute();

    	// update zone serial
    	$q = $dbh->prepare("UPDATE domains SET serial = ".$actual_serial.
    	" WHERE domain = '".$_POST['domain'].".'");
    	$q->execute();

      // return json wih added subdomain data
      $q = $dbh->prepare("SELECT s.id, subdomain, domain FROM subdomains s, domains d WHERE s.subdomain = '".$_GET['subd']."' AND d.domain = (SELECT domain FROM domains WHERE id = s.domain_id)");
      $q->execute();
      $res = $q->fetch();

      $domain = substr($res['domain'], 0, -1);
      $json = array(
        "id" => $res['id'],
        "subdomain" => $res['subdomain'],
        "domain" => $domain
      );
      header('Content-Type: application/json');
      echo json_encode($json);
    }
    if(isset($_GET['id']) && $_SERVER['REQUEST_METHOD'] != 'PUT') {
      $q = $dbh->prepare(
      "SELECT s.id, subdomain, domain FROM subdomains s, domains d WHERE s.id = ".$_GET['id']." AND d.domain = (SELECT domain FROM domains WHERE id = s.domain_id)");
      $q->execute();
      $res = $q->fetch();
      $json_resp = array(
        "id" => $res['id'],
        "subdomain" => $res['subdomain'],
        "domain" => $res['domain']
      );
      header('Content-Type: application/json');
      echo json_encode($json_resp);
    }
    if($_SERVER['REQUEST_METHOD'] == 'PUT' && !isset($_GET['domain'])) {
      parse_str(file_get_contents("php://input"), $json);

      $q = $dbh->prepare("UPDATE subdomains SET subdomain = '".$json['domain']."' WHERE id = ".$_GET['id']);
      $q->execute();

      $serial = $tool->calculateSerial($json['basedomain']);
      $q = $dbh->prepare("UPDATE domains SET serial = ".$serial. " WHERE domain = '".$json['basedomain'].".'");
      $q->execute();

      $q = $dbh->prepare("SELECT s.id, subdomain, domain FROM subdomains s, domains d WHERE s.id = ".$_GET['id']." AND d.domain = (SELECT domain FROM domains WHERE id = s.domain_id)");
      $q->execute();
      $res = $q->fetch();
      $basedom = substr($res['domain'], 0, -1);

      $json = array(
        "id" => $res['id'],
        "subdomain" => $res['subdomain'],
        "domain" => $basedom
      );
      header('Content-Type: application/json');
      echo json_encode($json);
    }
  }
?>
