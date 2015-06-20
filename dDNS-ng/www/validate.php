<?
   include_once './inc/config.php';
   $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
   $dbh = new PDO($dsn, LOGIN, PASS);
   $str = '';
   $field = '';
   $isValid = '';
   
	switch($_POST['type']) {
		case 'login' : $str = $_POST['login']; $field = $_POST['type']; break;
		case 'email' : $str = $_POST['email']; $field = $_POST['type']; break;
    }
	$q = $dbh->prepare("SELECT id FROM users WHERE ".$field." = '".$str."'");
	$q->execute();
	if($q->rowCount() > 0)
		$isValid = false;
	else
		$isValid = true;
	echo json_encode(array('valid' => $isValid));
?>
