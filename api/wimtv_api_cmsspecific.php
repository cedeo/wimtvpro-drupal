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
    return variable_get("basePathWimtv");
}

?>
