<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 14.32
 */
function wimtvpro_help($path, $arg) {
    /**
     * Visualizza il link che rimanda alla pagina di supporto.
     */
    if ($path == "admin/help#wimtvpro") {
        return render_template('templates/help.php');
    }
}
