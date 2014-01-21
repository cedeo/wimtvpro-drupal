<h2>Introduction</h2>
<p>The Wim TV Pro module enhances your Drupal website by converting it into a full video asset management and publishing platform.
   This module is particularly useful on portals where video plays a major role (Web TVs, news portals, etc.), simplifying, improving and speeding up all video management and publishing operations.
   Editors can upload videos directly from their Drupal site to the <a href="http://www.wim.tv" title="www.wim.tv" rel="nofollow">www.wim.tv</a> video platform,
   publish them on any node having a Body field, and stream them to the visitors directly from the cloud.</p>
<h2>Your webtv</h2>
<p>By <a href='http://www.wim.tv/wimtv-webapp/userRegistration.do' target='_new'>registering an account on Wim.tv</a>, Web TVs can:</p>
<ul>
    <li><strong>Import their videos</strong>, giving each of them a license chosen between a Creative Commons professional license or a Revenue Generating professional license (see the <a href='http://www.wim.tv/wimtv-webapp/licenseAbout.do' target='_new'>&ldquo;License for the Video&rdquo;</a> area for more details);</li>
    <li><strong>Create on demand schedules</strong>, manage and broadcast them both within and outside Wim.tv on its website (via embedded player);</li>
    <li><strong>Sell videos</strong>, making them available on the marketplace with an associated professional license for distribution and/or by setting a threshold payment for viewing;</li>
    <li><strong>Create Video Contests</strong>, specifying the categories and the type of content they want. The contest allows Web TVs to acquire new video contents to add to their schedules</li>
    <li><strong>Take part in Video Contests</strong>, consulting those activated by Web TVs and proposing their own videos to the selections.</li>
</ul>
<h2>Configuration</h2>
<p>Go to <a href="<?php echo url('admin/config/wimtvpro') ?>">WimTv configuration</a> to find all the configuration options.<br />
    To set your username and password of WimTV registration.</p>

<h2>Add a new video</h2>
<p>Go to <a href="<?php echo url('admin/config/wimtvpro/upload') ?>">Upload video</a><br />
    In this page you can add your WmTV videos and automatically they are uploaded to the WimTV server.<br />
    You can insert a title and a description of this site.<br />
</p>

<h2>Your videos</h2>
<p>Go to <a href="<?php echo url('admin/config/wimtvpro/wimbox') ?>">WimBox</a><br />
    In this page you can view your videos uploaded with this module, to see the work done within the WimTV site you should click "synchronize".<br />
    You can:
<ul>
    <li>Publish your video and assign the type of license (free, creative commons or Pay per view)</li>
    <li>Unpublish your video</li>
    <li>Play your video</li>
</ul>
</p>

<h2>Your Streamings</h2>
<p>Go to <a href="<?php echo url('admin/config/wimtvpro/wimvod') ?>">WimVod</a><br />
    In this page you can view your videos moved to WimVod and you can syncronize videos with WimTV.<br />
    You can:
<ul>
    <li>Unpublish your video</li>
    <li>Play your video</li>
    <li>Organize the order of videos</li>
</ul>
</p>

<h2>Your personal details of WimTV</h2>
<p>Go to <a href="<?php echo  url('admin/config/wimtvpro/mypersonalBlock') ?>">My personal detail</a>
    and you can change the visualization of your personal information (entered at the time of registration on WimTV) in a block "Block User Profile" block.</p>

<h2>View all videos in WimVod (page and block)</h2>
<p>You can show your videos moved to WimVod in the page "WimVod Video" or in the block "Block list video WimVod".</p>

<h2>Add your videos in WimVod to content page</h2>
<p>When you create a content page you view a block called "WimVod Video". In this block you can add or remove videos that you want to show.
    The videos are added in the body text like BBCODE ([wimtv]code[/wimtv]).<br />
    In the page to be created you will see the player (JWPLAYER) with the video.</p>


<h2>Create event live</h2>
<p>Go to <a href="<?php echo  url('admin/config/wimtvpro/wimlive') ?>">WimLive</a> and you create and view your future event.</p>