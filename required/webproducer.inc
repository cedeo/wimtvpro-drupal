<?php
  
function wimtvproWebProducer() {
    drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/swfObject/swfobject.js');
    drupal_add_css("
        #header{display:none;}
         #footer-wrapper{display:none;}
         #breadcrumb{display:none;}
         .sidebar{display:none!important}"
        ,'inline');
    $id2 = arg(5);
    $embedded = apiEmbeddedLive($id2);
    $arrayjson_live = json_decode($embedded);
    $url =  $arrayjson_live->url;
    $title = $arrayjson_live->name;

    $stream_url = explode ("/",$url);
    $stream_name = $stream_url[count($stream_url)-1];
    $url = "";
    for ($i=1;$i<count($stream_url)-1;$i++){
        $url .= $stream_url[$i] . "/";
    }
    $url = $stream_url[0] . "/" . $url;
    $url = substr($url, 0, -1);
    $user = json_decode(apiGetProfile());
    $live_password = $user->liveStreamPwd;

    $args = array('url' => $url, 'stream_name' => $stream_name, 'live_pwd' => $live_password);
    return render_template('templates/producer.php', $args);
}