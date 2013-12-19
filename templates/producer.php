<div id="page">	<h1>Producer</h1>
    <p>On this page you can view the video you\'re broadcasting live. Keep it open during the whole transmission.</p>
    <div  class="pageproducer">
        <div id="producer" ></div>
    </div>
</div>

<script type="text/javascript">
    jQuery(document).ready(function() {
        var url_pathPlugin ="<?php echo drupal_get_path('module', 'wimtvpro') ?>";
        var xiSwfUrlStr = url_pathPlugin  + "/jquery/swfObject/playerProductInstall.swf";
        console.log(xiSwfUrlStr );
        var flashvars = {};
        var params = {};
        params.quality = "high";
        params.bgcolor = "#ffffff";
        params.allowscriptaccess = "sameDomain";
        params.allowfullscreen = "true";
        var attributes = {};
        attributes.align = "left";

        swfobject.embedSWF(url_pathPlugin  + "/jquery/swfObject/producer.swf", "producer", "640", "480", "11.4.0",xiSwfUrlStr, flashvars, params, attributes );
        setTimeout(function () {
            producer = jQuery("#producer")[0];
            console.log(producer);

            producer.setCredentials("<?php echo variable_get("userWimtv") ?>", "<?php echo variable_get("passWimtv") ?>");
            producer.setUrl(decodeURIComponent("<?php echo $url ?>"));
            producer.setStreamName("<?php echo $stream_name ?>");
            producer.setStreamWidth(640);
            producer.setStreamHeight(480);
            producer.connect();
        }, 1000);
    });
</script>