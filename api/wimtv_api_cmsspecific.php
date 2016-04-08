<?php

/**
 * Written by Netsense s.r.l. 2015
 * 
 * CMS: DRUPAL
 */

function cms_getWimtvUser() {
    return variable_get("userWimtv");
}

function cms_getWimtvPwd() {
    return variable_get("passWimtv");
}

function cms_getWimtvApiUrl() {
    global $WIMTV_API_HOST;
    return $WIMTV_API_HOST;
}

function cms_getWimtvApiProductionUrl() {
    return variable_get("basePathWimtv");
}

function cms_getWimtvApiTestUrl() {
    return "http://peer.wim.tv/wimtv-webapp/rest/";
}

function cms_getName(){
    return "Drupal";
}

?>
