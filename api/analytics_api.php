<?php
/**
 * Written by walter at 30/10/13
 */
include_once("api.php");

use Api\Api;


function initAnalytics($host, $username, $password) {
    Api::initAnalyticsApi($host, $username, $password);
}

function getAnalytics() {
    return Api::getAnalyticsApi();
}

function analyticsGetStreams($from="", $to="") {
    $apiAccessor = getAnalytics();
    if ($from == "" && $to == "")
        $request = $apiAccessor->getRequest("users/" . $apiAccessor->username . "/streams");
    else
        $request = $apiAccessor->getRequest("users/" . $apiAccessor->username . "/streams?from=" . $from . "&to=" . $to);

    return $apiAccessor->execute($request);
}

function analyticsGetUser($from="", $to="") {
    $apiAccessor = getAnalytics();
    if ($from == "" && $to == "")
        $request = $apiAccessor->getRequest("users/" . $apiAccessor->username);
    else
        $request = $apiAccessor->getRequest("users/" . $apiAccessor->username . "?from=" . $from . "&to=" . $to);
    return $apiAccessor->execute($request);
}

function analyticsGetPacket() {
    $apiAccessor = getAnalytics();
    $request = $apiAccessor->getRequest("users/" . $apiAccessor->username . "/commercialPacket/usage");
    return $apiAccessor->execute($request);
}

initAnalytics("http://stats.wim.tv/api/", variable_get("userWimtv"), null);

?>