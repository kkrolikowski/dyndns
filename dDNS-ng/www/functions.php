<?php
  class Toolkit {
     public $retval = 0;
     public $token;

     public function checkPass($hash, $pass) {
        $salt = substr($hash, 0, 12);
        $newhash = crypt($pass, $salt);
        if(strcmp($newhash, $hash) == 0)
           $this->retval = 1;
        else
           $this->retval = 0;

        return $this->retval;
     }
     public function genToken() {
       $chars = '0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM';
       $this->token = substr(str_shuffle($chars), 0, 24);

       return $this->token;
     }
  }
?>
