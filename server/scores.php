<?php
$host = "thomassteinke.db";
$username = "thomassteink";
$password = "2bORNOT2b";
$database = "rgbzero";
$mysqli = mysqli_connect($host, $username, $password, $database);
if (mysqli_connect_errno()) {
   die("Failed to connect to MySQL: " . mysqli_connect_error());
}

$method = $_SERVER['REQUEST_METHOD'];
if ($method == 'GET') {
   $amount = 5;
   if ($_GET['amt']) {
      $amount = $_GET['amt'];
   }

   if ($_GET['level']) {
      $level = $_GET['level'];
      $objQuery = $mysqli->prepare("SELECT name, score FROM scores WHERE level = ? ORDER BY score DESC LIMIT ?");
      $objQuery->bind_param("si", $level, $amount);
   }
   else {
      $objQuery = $mysqli->prepare("SELECT name, score FROM scores WHERE 1 ORDER BY score DESC LIMIT ?");
      $objQuery->bind_param("i", $amount);
   }
   $objQuery->execute();
   $result = $objQuery->get_result();
   $top5 = array();
   while (($obj = $result->fetch_assoc()) && count($top5) < 5) {
      array_push($top5, $obj['name'] . " " . $obj['score']);
   }

   echo join("\n", $top5);
}
else if ($method == 'POST') {
   $score = $_POST['score'];
   $level = $_POST['level'];
   $name = $_POST['name'];

   if ($name == "" || $score == 0 || $level == "") {
      header('HTTP/1.1 400 Bad Request');
      header('Content-type: text/html');
   }
   else {
      $objQuery = $mysqli->prepare("INSERT INTO scores (name, score, level) VALUES (?, ?, ?)");
      $objQuery->bind_param("sis", $name, $score, $level);
      $objQuery->execute();

      header('HTTP/1.1 200 OK');
      header('Content-type: text/html');
   }
}
?>