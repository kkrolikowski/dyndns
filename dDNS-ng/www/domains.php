<?php
  session_start();
  include_once './inc/config.php';
  include 'functions.php';

  $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
  $dbh = new PDO($dsn, LOGIN, PASS);
  $size = 0;

  if(isset($_SESSION['userlogin'])) {
    $q = $dbh->prepare("SELECT id FROM users WHERE login = '".$_SESSION['userlogin']."' AND role = 'admin'");
    $q->execute();
    if($q->rowCount() > 0) {
      if($_SERVER['REQUEST_METHOD'] == 'PUT') {
        parse_str(file_get_contents("php://input"), $json);

        foreach($json['data']['type'] as $r) {
          $size++;
        }
        $q = $dbh->prepare("SELECT id FROM domains WHERE domain = '".$json['data']['origin']."'");
        $q->execute();
        if($q->rowCount() == 0) {
          $q = $dbh->prepare(
          "INSERT INTO domains(domain,status,user_id,owner,ttl,admin_contact,master_dns,serial) VALUES".
          "('".$json['data']['origin']."', 'private', (SELECT id FROM users WHERE login = '".$_SESSION['userlogin']."'), '".
          $_SESSION['userlogin']."', ".$json['data']['ttl'].", '".$json['data']['admin']."', '".$json['data']['masterdns'].
          "', ".$json['data']['serial']. ")"
          );
          $q->execute();
          for($i = 0; $i < $size; $i++) {
            $q = $dbh->prepare(
            "INSERT INTO subdomains(user_id,domain_id,subdomain,ip,type) VALUES".
            "((SELECT id FROM users WHERE login = '".$_SESSION['userlogin']."'), " .
            "(SELECT id FROM domains WHERE domain ='".$json['data']['origin']."'), '" .
            $json['data']['subdomain'][$i]."', '".$json['data']['ip_or_name'][$i]."', '" .
            $json['data']['type'][$i] . "')"
            );
            $q->execute();
          }
        }
        else
          header('X-Message: Domain exists',  true, 406);
      }
    }
  }
?>
