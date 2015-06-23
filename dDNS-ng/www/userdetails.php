<?php
   include_once './inc/config.php';
   $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
   $dbh = new PDO($dsn, LOGIN, PASS);
   
   if(isset($_GET['id'])) {
	   $q = $dbh->prepare("SELECT id,name,login,email,subdomain FROM users WHERE id = ".$_GET['id']);
	   $q->execute();
	   $res = $q->fetch();
	   $json_resp = array(
			"id" => $res['id'],
			"login" => $res['login'],
			"name" => $res['name'],
			"email" => $res['email'],
			"subdomain" => $res['subdomain']
		);
		header('Content-Type: application/json');
		echo json_encode($json_resp);
	}
	if(isset($_POST['id'])) {
		$q = $dbh->prepare("DELETE FROM users WHERE id = ".$_POST['id']);
		$q->execute();
	}
?>