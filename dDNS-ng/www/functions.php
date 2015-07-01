<?php
  include_once './inc/config.php';

  class Toolkit {
     public $retval = 0;
     public $token;
     public $md5pass;

     public function checkPass($hash, $pass) {
        $salt = substr($hash, 0, 12);
        $newhash = crypt($pass, $salt);
        if(strcmp($newhash, $hash) == 0)
           $this->retval = 1;
        else
           $this->retval = 0;

        return $this->retval;
     }
     public function genToken($len) {
       $chars = '0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM';
       $this->token = substr(str_shuffle($chars), 0, $len);

       return $this->token;
     }
     public function encryptpass($pass) {
       $this->md5pass = crypt($pass, "$1$" . base64_encode(mcrypt_create_iv(6)));

       return $this->md5pass;
     }
     public function notify($mailto, $subject, $message) {
       $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
       $dbh = new PDO($dsn, LOGIN, PASS);

       $mailq = $dbh->prepare("INSERT INTO mailqueue(mailto,mailfrom,subject,reply_to,x_mailer,message) VALUES('"
       .$mailto."','".ADM_EMAIL."','[dDNS] ".$subject."','".ADM_EMAIL."','dDNS messanger','".$message."')");
       $mailq->execute();
     }
  }
?>
