<?php
  class Toolkit {
     public $retval = 0;

     public function checkPass($hash, $pass) {
        $salt = substr($hash, 0, 12);
        $newhash = crypt($pass, $salt);
        if(strcmp($newhash, $hash) == 0)
           $this->retval = 1;
        else
           $this->retval = 0;
        
        return $this->retval;
     }
  }
?>
