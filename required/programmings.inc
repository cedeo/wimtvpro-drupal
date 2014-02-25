<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 24/02/14
 * Time: 14.43
 * To change this template use File | Settings | File Templates.
 */

function wimtvpro_programmings_api() {


    header('Access-Control-Allow-Origin: *');
    header('Content-type: application/json');
    header('Response: HTTP/1.1 200 OK');

    $api = $_GET['api'];
    switch ($api) {
        case "calendar":
            calendar();
            break;
        case "addItem":
            addItem();
            break;
        case "pool":
            pool();
            break;
        case "currentProgramming":
            currentProgramming();
            break;
        case "programmings":
            programmings();
            break;
        case "removeItem":
            removeItem();
            break;
        case "deleteItems":
            deleteItems();
            break;
        case "updateItem":
            updateItem();
            break;

        default:
            echo "ERRORE";
    }
}

function calendar() {
    $qs=$_SERVER['QUERY_STRING'];
    parse_str($qs, $qs_array);
    $progId = $qs_array['progId'];

    $response = apiGetCalendar($progId, $qs);
    echo $response;
    die();
}

function addItem() {
    $qs=$_SERVER['QUERY_STRING'];
    parse_str($qs, $qs_array);
    $progId = $qs_array['progId'];

    $response = apiAddItem($progId, $_POST);
    echo $response;
    die();
}

function pool() {
    $qs=$_SERVER['QUERY_STRING'];
    parse_str($qs, $qs_array);
    //$progId = $qs_array['progId'];
    $response = apiProgrammingPool();
    echo $response."\n";

    $arrayjsonst = json_decode($response);
    //echo $arrayjsonst->id."\n";

    die ();
}

function currentProgramming() {

    $qs=$_SERVER['QUERY_STRING'];
    parse_str($qs, $qs_array);
    $progId = $qs_array['progId'];
    $response = apiGetCurrentProgrammings($qs);
    echo $response;
    //echo "identifier:" .   $arrayjsonst->identifier;
    die();
}

function programmings() {
    $response = apiPostProgrammings($_POST);
    echo $response;
    die();
}

function removeItem() {
    $qs=$_SERVER['QUERY_STRING'];
    parse_str($qs, $qs_array);
    $progId = $qs_array['progId'];

    $response = apiRemoveItemProgramming($progId,$qs);
    echo $response;
    die ();
}

function deleteItems() {
    $qs=$_SERVER['QUERY_STRING'];
    parse_str($qs, $qs_array);
    $progId = $qs_array['progId'];
    $itemId = $qs_array['itemId'];

    $response = apiDeleteItems($progId, $itemId);
    echo $response;
    die();
}

function updateItem() {
    $qs=$_SERVER['QUERY_STRING'];
    parse_str($qs, $qs_array);
    $progId = $qs_array['progId'];
    $itemId = $qs_array['itemId'];

    $response = apiUpdateItems($progId, $itemId, $_POST);
    echo $response;
    die();
}
