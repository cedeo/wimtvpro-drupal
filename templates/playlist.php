<div class="help"><a href="http://support.wim.tv/?cat=5" target="_new">Help</a></div>
<script type="text/javascript">
    jQuery(document).ready(function(){

        jQuery(".playlist input.title").click(function() {
            jQuery(this).parent().parent().children("td.action").children(".icon_modTitlePlay").show();
        });

        jQuery(".icon_viewPlay").click(function () {
            var id= jQuery(this).parent().attr("rel");
            //jQuery(this).colorbox({href:  url_pathPlugin + "pages/embeddedPlayList.php?id=" + id});
        });

        jQuery(".icon_createPlay").click(function() {
            var nameNewPlaylist = jQuery(this).parent().parent().children("td").children("input").val();
            //ID = playlist_##
            var count = jQuery(".playlist").size();
            count  = count + 1;
            //add to DB
            jQuery.ajax({
                context: this,
                url: "<?php echo $urlCallAjax ?>",
                type: "GET",
                data:{
                    namePlayList : nameNewPlaylist,
                    namefunction: "createPlaylist"
                },
                success: function(response) {
                    location.reload();
                },
                error: function(jqXHR, textStatus, errorThrown){alert(errorThrown);}
            });
        });

        jQuery(".icon_modTitlePlay").click(function() {
            var nameNewPlaylist = jQuery(this).parent().parent().children("td").children("input").val();
            //ID = playlist_##
            var idPlayList = jQuery(this).parent().attr("rel");

            //add to DB
            jQuery.ajax({
                context: this,
                url: "<?php echo $urlCallAjax ?>",
                type: "GET",
                data:{
                    idPlayList : idPlayList,
                    namePlayList : nameNewPlaylist,
                    namefunction: "modTitlePlaylist"
                },
                success: function(response){
                    location.reload();
                },
                error: function(jqXHR, textStatus, errorThrown){alert(errorThrown);}
            });
        });

        jQuery(".icon_deletePlay").click(function() {
            var nameNewPlaylist = jQuery(this).parent().parent().children("td").children("input").val();
            //remove from DB
            var idPlayList = jQuery(this).parent().attr("rel");
            //add to DB
            jQuery.ajax({
                context: this,
                url: "<?php echo $urlCallAjax ?>",

                type: "GET",
                data:{
                    idPlayList : idPlayList,
                    namefunction: "removePlaylist"
                },
                success: function(response){
                    location.reload();
                },
                error: function(jqXHR, textStatus, errorThrown){alert(errorThrown);}
            });
        });
    });
</script>
<div id="view_stream" class="view">
    <table class="wp-list-table widefat fixed posts" style="text-align:center;">
        <tr>
            <th class="manage-column column-title">Title Playlist</th>
            <th class="manage-column column-title">Action</th>
        </tr>
        <?php foreach ($playlists as $record) { ?>
            <tr>
                <td class="playlist">
                    <a href="<?php echo url('admin/config/wimtvpro/playlist/modify/' . $record->id) ?>">
                        <span class="icon_viewPlay"></span>
                    </a>
                    <input class="title" type="text" value="<?php echo $record->name ?>"/>
                    <span class="counter">(<?php echo $record->countVideo ?>)</span>
                </td>
                <td class="action"  rel="<?php echo $record->id ?>">
                    <span class="icon_modTitlePlay"></span>
                    <span class="icon_deletePlay"></span>
                </td>
            </tr>
        <?php } ?>
        <tr>
            <td  class="playlistNew">
                <input type="text" value="Playlist <?php echo $count ?>" />
            </td>
            <td>
                <span class="icon_createPlay"></span>
            </td>
        </tr>
    </table>
</div>