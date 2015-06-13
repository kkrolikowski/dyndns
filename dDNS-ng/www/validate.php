<?
   include_once './inc/config.php';
   $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
   $dbh = new PDO($dsn, LOGIN, PASS);

   $q = $dbh->prepare("SELECT login FROM users WHERE login = '".$_POST['login']."'");
   $q->execute();
   if($q->rowCount() > 0) {
     echo json_encode(array('valid' => false));
   }
   else {
     echo json_encode(array('valid' => true));
   }
?>
