<?php
  include_once './inc/config.php';
  include('Mail.php');
  $dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
  $dbh = new PDO($dsn, LOGIN, PASS);
  $mail = Mail::factory("mail");

  $q = $dbh->prepare("SELECT * FROM mailqueue");
  $q->execute();
  if($q->rowCount() > 0) {
    while ($res = $q->fetch()) {
      $headers = array(
        "From" => $res['mailfrom'],
        "Subject" => $res['subject'],
        "Reply-to" => $res['reply_to'],
        "X-Mailer" => $res['x_mailer']
      );
      $mail->send($res['mailto'], $headers, $res['message']);
      $qdel = $dbh->prepare("DELETE FROM mailqueue WHERE id =".$res['id']);
      $qdel->execute();
    }
  }
?>
