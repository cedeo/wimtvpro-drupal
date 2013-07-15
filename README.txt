Drupal WimTVPro module:
------------------------
Maintainers:
  WIMLABS
  Simona Guala
  Sergio Matone
  (http://www.wimlabs.com)

Requires - Drupal 7


Overview:
--------
WimTVPro is the video plugin that adds several features to manage and publish video on demand, video playlists and stream live events on your website.


Features:
---------

With WimTVPro you can extend WordPress with a powerful plugin that allows you to organize, store, publish and stream video in posts, pages or widgets to any location within your website/blog.
Storage and bandwidth used for the video is provided by WimTV, the innovative platform for live and on demand video streaming. To use the plugin, you must have a Web TV account on WimTV. Registration is free of charge. For sign up http://www.wim.tv/wimtv-webapp/userRegistration.do?execution=e1s1

WimTVPro allows its users to:

Connect (with your own credentials) to your account on WimTV
Upload new videos, describe and publish
For each video published, set of access conditions (free, creative commons, pay per view)
Given a list of videos, publish those of choice on your website
Change with drag-and-drop the order of presentation of videos on a web page
Insert videos anywhere on the page (e.g. in posts, pages or in widgets)
Choose the skin and the size of the video player
Choose to make videos visible to everyone or only to certain users
Create a new video playlist and insert into the pages and posts of the site
Create live streaming events to be published on the pages of your site
Synchronize videos with your WimTV account (if you posted a video with some conditions, these are also updated on WimTV)
The plugin is integrated into the menu and is divided into five section, to always have an ordered control of content. For more details about WimTVPro's functionalities visit http://wimtvpro.tv/functionalities.html

Demo user is http://research.cedeo.net/wordpress For request a demo user, please send an email to info@wimlabs.com

Installation:
------------
1. Download and unpack the Wim Video Promodule directory in your modules folder
   (this will usually be "sites/all/modules/").
2. Go to "Administer" -> "Modules" and enable the module.


Configuration:
-------------
IMPORTANT - First go to WimTVPro --> WimTV configuration to set up all the configuration options and your WimTV's credentials. For sign up on WimTV http://www.wim.tv/wimtv-webapp/userRegistration.do?execution=e1s1


Add a new video:
----------------------------------------
Go to "Configuration" -> -> "Wimtv configuration" -> -> "Upload video"
In this page you can add your WmTV videos and automatically they are uploaded to the WimTV server.
You can insert a title and a description of this site.
COMING SOON: insert category and subcategory.


View video and syncronize:
----------------------------------
Go to "Configuration" -> -> "WimTV configuration" -> -> "My Media"
In this page you can view your videos uploaded with this module, to see the work done within the WimTV site you should
click "synchronize".
You can:
* Publish your video and assign the type of license
* Unpublish your video
* Play your video


View video into My Streaming:
----------------------------------
Go to "Configuration" -> -> "Wimtv configuration" -> -> "My Streaming"
In this page you can view your videos moved to My Streaming and you can syncronize videos with WimTV.
You can:
* Unpublish your video
* Play your video
* Organize the order of videos


View your personal detaisl of WimTV and block:
----------------------------------------
Go to "Configuration" -> -> "Wimtv configuration" -> -> "My personal detail"
You can change the visualization of your personal information (entered at the time of registration on WimTV)
To activate the "Block User Profile" block go to "Structure" -> -> "Blocks"


View all videos in My Streaming (page and block):
----------------------------------------
You can show your videos moved to My Streaming in the page "My Streaming Video" or in the block "Block list video My Streaming".


Add your videos in My Streaming to content page:
----------------------------------------
When you create a content page you view a block called "My Streaming Video". In this block you can add or remove
videos that you want to show. The videos are added in the body text like BBCODE ([wimtv]code[/wimtv]).
In the page to be created you will see the player (JWPLAYER) with the video.



Last updated:
------------


Contacts:
------------
For product support and general information: Riccardo Chiariglione riccardo@wimlabs.com WimLabs srl

Visit the plugin's website: http://wimtvpro.tv


Required:
------------
Extension server php "php_curl.dll"
Verify php.ini install curl
For install "sudo apt-get install php5-curl"
