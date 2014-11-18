
<?php

define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
chdir(realpath(DRUPAL_ROOT));
require_once("./includes/bootstrap.inc");
drupal_bootstrap(7);  //DRUPAL_BOOTSTRAP_FULL
include_once(realpath("../../api/wimtv_api.php"));
?>

