<?php
  include_once './inc/config.php';

  class Toolkit {
     public $retval = 0;
     public $token;
     public $md5pass;
     public $newserial;

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
     public function calculateSerial($domain) {
       $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
       $dbh = new PDO($dsn, LOGIN, PASS);

       $dayStartSerial = date('Ymd') . "00";
       $q = $dbh->prepare("SELECT serial FROM domains WHERE domain = '".$domain.".'");
       $q->execute();
       $res = $q->fetch();
       $actual_serial = $res['serial'];

       if(intval($actual_serial) < intval($dayStartSerial)) {
         $newserial = $dayStartSerial;
       }
       else {
         $newserial = intval($actual_serial) + 1;
       }
       return $newserial;
     }
  }
?>
