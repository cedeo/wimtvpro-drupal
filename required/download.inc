<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 21/01/14
 * Time: 17.07
 * To change this template use File | Settings | File Templates.
 */

function wimtvpro_download() {
    $code = explode("download/", request_path());
    if (count($code)>1) {
        $host_id = $code[1];
        $file = apiDownload($host_id);
        $headers = $file->headers;

        $response_headers = array('content-type', 'content-disposition', 'accept-ranges', 'content-length');

        foreach ($response_headers as $header) {
            header($header . ": " . $headers[$header]);
        }

        echo($file);

    }
    echo "";
    die();
}