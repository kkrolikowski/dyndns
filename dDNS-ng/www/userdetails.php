<?php
   include_once './inc/config.php';
   $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
   $dbh = new PDO($dsn, LOGIN, PASS);
   
   $q = $dbh->prepare("SELECT name,login,email,subdomain FROM users WHERE id = ".$_GET['id']);
   $q->execute();
   $res = $q->fetch();
   $json_resp = array(
		"login" => $res['login'],
		"name" => $res['name'],
		"email" => $res['email'],
		"subdomain" => $res['subdomain']
	);
	header('Content-Type: application/json');
	echo json_encode($json_resp);
?>