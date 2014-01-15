<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 12.48
 * To change this template use File | Settings | File Templates.
 */

function render_template($template_file, $args=array()) {
    ob_start();
    extract($args);
    require $template_file;
    return ob_get_clean();
}