<?php
	session_start();
   include_once './inc/config.php';
   $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
   $dbh = new PDO($dsn, LOGIN, PASS);
   if(isset($_SESSION['userlogin'])) {
		$q = $dbh->prepare("SELECT id FROM users WHERE login = '".$_SESSION['userlogin']."' AND role = 'admin'");
		$q->execute();
		if($q->rowCount() > 0) {
		   if(isset($_GET['id']) && $_SERVER['REQUEST_METHOD'] != 'PUT') {
			   $q = $dbh->prepare("SELECT id,name,login,active,role,email,subdomain FROM users WHERE id = ".$_GET['id']);
			   $q->execute();
			   $res = $q->fetch();
			   $json_resp = array(
					"id" => $res['id'],
					"login" => $res['login'],
					"name" => $res['name'],
					"active" => $res['active'],
					"role" => $res['role'],
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
			if(isset($_GET['enable'])) {
				$q = $dbh->prepare("UPDATE users SET active = 1 WHERE id = ".$_GET['enable']);
				$q->execute();
			}
			if($_SERVER['REQUEST_METHOD'] == 'PUT') {
				parse_str(file_get_contents("php://input"), $json);
				$q = $dbh->prepare("UPDATE users SET name = '".$json['name']."', login = '".$json['login']."', role = '".$json['role'].
				"', active = ".$json['active'].", subdomain = '".$json['subdomain']."', email = '".$json['email']."' WHERE id = ".$_GET['id']);
				$q->execute();
				$q = $dbh->prepare("SELECT id,name,login,active,role,email,subdomain FROM users WHERE id = ".$_GET['id']);
				$q->execute();
				$res = $q->fetch();
				$json_resp = array(
				"id" => $res['id'],
				"login" => $res['login'],
				"name" => $res['name'],
				"active" => $res['active'],
				"role" => $res['role'],
				"email" => $res['email'],
				"subdomain" => $res['subdomain']
			);
			header('Content-Type: application/json');
			echo json_encode($json_resp);
			}
		}
	}
?>
