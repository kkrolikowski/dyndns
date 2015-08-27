<?php
  session_start();
  include_once './inc/config.php';
  include 'functions.php';

  $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
  $dbh = new PDO($dsn, LOGIN, PASS);
  $tool = new Toolkit;

  if(isset($_SESSION['userlogin'])) {
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
    if(isset($_GET['id']) && $_SERVER['REQUEST_METHOD'] != 'PUT') {
      $q = $dbh->prepare(
      "SELECT subdomain, domain FROM subdomains s, domains d WHERE s.id = ".$_GET['id']." AND d.domain = (SELECT domain FROM domains WHERE id = s.domain_id)");
      $q->execute();
      $res = $q->fetch();
      $json_resp = array(
        "subdomain" => $res['subdomain'],
        "domain" => $res['domain']
      );
      header('Content-Type: application/json');
      echo json_encode($json_resp);
    }
    if($_SERVER['REQUEST_METHOD'] == 'PUT') {
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
