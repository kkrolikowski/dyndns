<?php
   include_once './inc/config.php';
   $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
   $dbh = new PDO($dsn, LOGIN, PASS);
   
   $q = $dbh->prepare("SELECT activate FROM users WHERE active = 0");
   $q->execute();
   if($q->rowCount() > 0) {
		$q = $dbh->prepare("UPDATE users SET active = 1 WHERE activate = '".$_GET['act']. "'");
		$q->execute();
		echo "Account activated";
   }
   else
		echo "Account is active or invalid token";
?>