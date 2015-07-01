<?php
	session_start();
	include_once './inc/config.php';
	include 'functions.php';

  $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
  $dbh = new PDO($dsn, LOGIN, PASS);
	$tool = new Toolkit;

   if(isset($_SESSION['userlogin'])) {
		$q = $dbh->prepare("SELECT id FROM users WHERE login = '".$_SESSION['userlogin']."' AND role = 'admin'");
		$q->execute();
		if($q->rowCount() > 0) {
		   if(isset($_GET['id']) && $_SERVER['REQUEST_METHOD'] != 'PUT') {
				$q = $dbh->prepare(
					"SELECT u.id, u.name,u.login, u.email, u.role, u.active, s.subdomain, d.domain ".
					"FROM subdomains s, domains d , users u where s.domain_id = d.id and u.id = s.user_id and s.user_id = ".$_GET['id']
				);
			   $q->execute();
			   $res = $q->fetch();
			   $json_resp = array(
					"id" => $res['id'],
					"login" => $res['login'],
					"name" => $res['name'],
					"active" => $res['active'],
					"role" => $res['role'],
					"email" => $res['email'],
					"subdomain" => $res['subdomain'],
					"domain" => $res['domain']
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
				print_r($json);
				parse_str(file_get_contents("php://input"), $json);

				$q = $dbh->prepare("UPDATE users SET name = '".$json['name']."', login = '".$json['login']."', role = '".$json['role'].
				"', active = ".$json['active'].", email = '".$json['email']."' WHERE id = ".$_GET['id']);
				$q->execute();

				$q = $dbh->prepare(
					"UPDATE subdomains SET subdomain = '".$json['subdomain']."' WHERE user_id = ".$_GET['id'].
					" AND domain_id = (SELECT id from domains where domain = '".$json['domain']."')");
				$q->execute();

				$q = $dbh->prepare(
					"SELECT u.id, u.name,u.login, u.email, u.role, u.active, s.subdomain, d.domain ".
					"FROM subdomains s, domains d , users u where s.domain_id = d.id and u.id = s.user_id and s.user_id = ".$_GET['id']
				);
				$q->execute();
				$res = $q->fetch();
				$json_resp = array(
				"id" => $res['id'],
				"login" => $res['login'],
				"name" => $res['name'],
				"active" => $res['active'],
				"role" => $res['role'],
				"email" => $res['email'],
				"subdomain" => $res['subdomain'],
				"domain" => $res['domain']
			);
			header('Content-Type: application/json');
			echo json_encode($json_resp);
		}
			if(isset($_GET['newpass'])) {
					$newpass = $tool->genToken(12);
					$md5pass = $tool->encryptpass($newpass);

					$q = $dbh->prepare("UPDATE users set pass = '".$md5pass."' WHERE id = ".$_GET['newpass']);
					$q->execute();
					$q = $dbh->prepare("SELECT name,login,email FROM users WHERE id = ".$_GET['newpass']);
					$q->execute();
					$res = $q->fetch();

					$message = 'Hello '. $res['name'] ."!\r\n\r\n".
					"Password to your account was reset by admin.\r\n\r\n".
					"New credentials:\r\n".
					"Login: ".$res['login']."\r\n".
					"Password: ".$newpass."\r\n\r\n".
					"--\r\n".
					"dDNS service";

					$tool->notify($res['email'], "Password reset", $message);
			}
		}
	}
?>
