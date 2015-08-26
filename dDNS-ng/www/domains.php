<?php
  session_start();
  include_once './inc/config.php';
  include 'functions.php';

  $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
  $dbh = new PDO($dsn, LOGIN, PASS);

  if(isset($_SESSION['userlogin'])) {
    if(isset($_GET['id']) && $_SERVER['REQUEST_METHOD'] != 'PUT') {
      $q = $dbh->prepare(
      "SELECT subdomain, domain FROM subdomains s, domains d WHERE s.id = ".$_GET['id']." AND d.domain = (SELECT domain FROM domains WHERE id = s.domain_id);");
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
    }
  }
?>
