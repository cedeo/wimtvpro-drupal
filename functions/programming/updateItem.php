<?php

include './bootstrap_functions.php';

header('Access-Control-Allow-Origin: *');
header('Content-type: application/json');
header('Response: HTTP/1.1 200 OK');

$qs = $_SERVER['QUERY_STRING'];
parse_str($qs, $qs_array);
$progId = $qs_array['progId'];
$itemId = $qs_array['itemId'];

$response = apiUpdateItems($progId, $itemId, $_POST);
echo $response;
die();
