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
    $objQuery = $mysqli->prepare("SELECT name, score FROM scores WHERE 1 ORDER BY score");
    $objQuery->execute();
      echo json_encode($objQuery->get_result()->fetch_object());  
}
else if ($method == 'POST') {
	
}
?>