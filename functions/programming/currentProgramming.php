<?php
include './bootstrap_functions.php';

$qs=$_SERVER['QUERY_STRING'];
parse_str($qs, $qs_array);
$progId = $qs_array['progId'];
$response = apiGetCurrentProgrammings($qs);
echo $response;
echo "identifier:" .   $arrayjsonst->identifier;

?>

