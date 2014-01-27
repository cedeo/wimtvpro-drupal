<div class="help"><a href="http://support.wim.tv/?cat=5" target="_new">Help</a></div>

<div class='action'>
    <span class='icon_sync0' title='Syncronize'><a href="javascript:void(0)" class='button'><?php echo t("Synchronize");?></a></span>
</div>


<div class="region region-help">
    <div id="block-system-help" class="block block-system">
    
    
        <div class="content">
            <p>
            
            <?php echo t("This page lets you manage the videos you want to publish.");?>
            
            </p>
        </div>
	</div>
</div>

<table class='items' id='TRUE'>
    <thead>
    <tr>
            <th>Video</th>
            <th><?php echo t("Status");?></th>
            <th>Download</th>
            <th><?php echo t("View");?></th>
            <th><?php echo t("Delete");?></th>
        </tr>
    </thead>
    <tbody>
        <?php echo $thumbs ?>
    </tbody>
</table>