<?php
	session_start();
   include_once './inc/config.php';
   $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
   $dbh = new PDO($dsn, LOGIN, PASS);
   if(isset($_SESSION['userlogin'])) {
		$q = $dbh->prepare("SELECT id FROM users WHERE login = '".$_SESSION['userlogin']."' AND role = 'admin'");
		$q->execute();
		if($q->rowCount() > 0) {
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
			if(isset($_GET['rm'])) {
				$q = $dbh->prepare("DELETE FROM users WHERE id = ".$_GET['rm']);
				$q->execute();
			}
		}
	}
?>