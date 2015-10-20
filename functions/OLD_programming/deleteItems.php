<?php
include './bootstrap_functions.php';

$qs=$_SERVER['QUERY_STRING'];
parse_str($qs, $qs_array);
$progId = $qs_array['progId'];
$itemId = $qs_array['itemId'];
header('Access-Control-Allow-Origin: *');
header('Content-type: application/json');
header('Response: HTTP/1.1 200 OK');

$response = apiDeleteItems($progId, $itemId);
echo $response;
die();


?>

