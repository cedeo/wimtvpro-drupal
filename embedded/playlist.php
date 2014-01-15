<html>
    <head>
        <script type='text/javascript'
                src='<?php echo $JwPlayerScript ?>'>
        </script>
    </head>
    <body>
        <div style='text-align:center;'>
            <div id='container'></div>
            <div id='container_playlist'></div>
            <script type='text/javascript'>
                jwplayer('container_playlist').setup({
                    'plugins': {
                        'sharing-3': {
                            'code': '<?php echo $embedded ?>'
                        }},
                    'repeat': '<?php echo $repeat ?>',
                    <?php if ($flash) { ?>
                    'flashplayer': '<?php echo $JwPlayerPath ?>',
                    <?php } ?>
                    'skin': '<?php echo $skin ?>',
                    'height': '<?php echo $height ?>',
                    'width': '<?php echo $width ?>',
                    'playlist': [<?php echo  $playlist ?>],
                    'playlist.position': 'right',
                    'playlist.size': '<?php echo $playlistSize  ?>'
                });
            </script>
        </div>
    </body>
</html>