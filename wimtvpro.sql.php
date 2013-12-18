<?php
/**
  * @file
  * SQL operation. //TODO: a me sembra che faccia tutt'altro....
  *
*/
header('Content-type: application/json');

$function = "";
$id="";
$acid="";
$ordina = "";
$credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");

if (isset($_GET['namefunction']))
$function= $_GET["namefunction"];
if (isset($_GET['id']))
$id = $_GET['id'];
if (isset($_GET['acquiredId']))
$acid = $_GET['acquiredId'];
if (isset($_GET['showtimeId']))
$stid = $_GET['showtimeId'];
if (isset($_GET['ordina']))
$ordina = $_GET['ordina'];

if (isset($_GET['namePlayList']))
$name= $_GET["namePlayList"];

if (isset($_GET['idPlayList']))
$idPlayList = $_GET["idPlayList"];


switch ($function) {
    case "putST":
        //Insert Video into mystreaming and into wim.tv streaming
        $licenseType= "";
        $paymentMode= "";
        $ccType= "";
        $pricePerView= "";
        $pricePerViewCurrency= "";

        if (isset($_GET['licenseType']))
            $licenseType = $_GET['licenseType'];
        if (isset($_GET['paymentMode']))
            $paymentMode = $_GET['paymentMode'];
        if (isset($_GET['ccType']))
            $ccType = $_GET['ccType'];
        if (isset($_GET['pricePerView']))
            $pricePerView = $_GET['pricePerView'];
        if (isset($_GET['pricePerViewCurrency']))
            $pricePerViewCurrency = $_GET['pricePerViewCurrency'];

        $param=array('licenseType'=>$licenseType,
            'paymentMode'=>$paymentMode,
            'ccType'=>$ccType,
            'pricePerView'=>$pricePerView,
            'pricePerViewCurrency'=>$pricePerViewCurrency
        );

        $response = apiPublishOnShowtime($id, $param);
        echo $response;
	 
        $state = "showtime";
        $array_response = json_decode($response);
        $sql = "UPDATE {wimtvpro_videos} SET state='" . $state . "' ,showtimeIdentifier='" . $array_response -> showtimeIdentifier . "' WHERE contentidentifier='" . $id . "'";
        $query = db_query($sql);

        break;

    case "putAcqST":
        $licenseType = "";
        $paymentMode = "";
        $ccType = "";
        $pricePerView  = "";
        $pricePerViewCurrency = "";

        if (isset($_GET['coId']))
            $acid = $_GET['coId'];
        if (isset($_GET['licenseType']))
            $licenseType = $_GET['licenseType'];
        if (isset($_GET['paymentMode']))
            $paymentMode = $_GET['paymentMode'];
        if (isset($_GET['ccType']))
            $ccType = $_GET['ccType'];
        if (isset($_GET['pricePerView']))
            $pricePerView = $_GET['pricePerView'];
        if (isset($_GET['pricePerViewCurrency']))
            $pricePerViewCurrency = $_GET['pricePerViewCurrency'];

        $params=array('licenseType'=>$licenseType,
            'paymentMode'=>$paymentMode,
            'ccType'=>$ccType,
            'pricePerView'=>$pricePerView,
            'pricePerViewCurrency'=>$pricePerViewCurrency
        );

        $state="showtime";

        $sql = "UPDATE {wimtvpro_videos} SET state='" . $state . "' WHERE contentidentifier='" . $id . "'";
        $query = db_query($sql);
        $response = apiPublishAcquiredOnShowtime($id, $acid ,$params);
        echo $response;
        break;

    case "removeST":
        //Remove into Mystreaming
        $state="";
        $num_updated = db_update('{wimtvpro_videos}') -> fields(array('position ' => '0',
                                                                      'state' => '',
                                                                      'showtimeIdentifier' => '',)) -> condition("contentidentifier", $id) -> execute();
        $response = apiDeleteFromShowtime($id, $stid);

        echo $response;
        break;

    case "StateViewThumbs":
        //Change the state of view thumbs (into block or page)
        $state = $_GET['state'];
        $num_updated = db_update('{wimtvpro_videos}')->fields(array('viewVideoModule' => $state)) ->condition("contentidentifier", $id)->execute();
        echo $state;
        break;

    case "ReSortable":
        //Change position of videos
        $list_video = explode(",", $ordina);
        foreach ($list_video as $position => $item) {
            $position = $position + 1;
            $num_updated = db_update('{wimtvpro_videos}')->fields(array('position ' => $position))->condition("contentidentifier", $item)->execute();
        }
        break;

    case "urlCreate":
        //Call API for create a event live url
        $response = apiCreateUrl(urlencode($_GET['titleLive']));
        echo $response;
        break;

    case "passCreate":
        //Call API for create a password for create a event live
        $response = apiChangePassword($_GET['newPass']);
        echo $response;
        break;

    case "RemoveVideo":
        //connect at API for upload video to wimtv
        $response = apiDeleteVideo($id);
        curl_close($ch);
        $arrayjsonst = json_decode($response);
        if ($arrayjsonst->result=="SUCCESS")
          $query = db_delete('{wimtvpro_videos}')->condition("contentidentifier", $id)->execute();
        echo $response;
        break;

	case "ModifyTitleVideo":
	    $num_updated = db_update('{wimtvpro_videos}')->fields(array('title' => $_GET["titleVideo"])) -> condition("vid", $id)->execute();
	    break;

	case "createPlaylist":
		$sql = "INSERT INTO {wimtvpro_playlist} (uid,listVideo,name,id) VALUES ('" . variable_get("userWimtv") . "' ,'','" . $name . "','" . time() . "')";
        $query = db_query($sql);
        break;
    
    case "modTitlePlaylist":
	    $num_updated = db_update('{wimtvpro_playlist}')->fields(array('name' => $name)) -> condition("id", $idPlayList)->execute();
        break;

	case "removePlaylist":
        //remove File
		$query = db_delete('{wimtvpro_playlist}')->condition("id", $idPlayList);
		echo $query->execute();
        break;

	case "downloadVideo":
		ini_set('max_execution_time', 300);
		ini_set("memory_limit","1000M"); 
		$credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
		$result = db_query("SELECT * FROM {wimtvpro_videos} WHERE contentidentifier = '" . $id . "'");
		$arrayStatusVideo = $result->fetchAll();
				
		$filename = "";
		$ext = "";
		if (count($arrayStatusVideo)>0){
			if (isset($arrayStatusVideo[0]->status)) {
				$filestatus = explode ("|",$arrayStatusVideo[0]->status);
				if (count($filestatus)>0){
					if (isset($filestatus[1])){
						$infoFile = explode (".",$filestatus[1]);
						$numeroCount = count($infoFile);
						$ext = $infoFile[$numeroCount-1];
						$filename = $infoFile[0];
						for ($i=1;$i<$numeroCount-1;$i++){
							$filename .= "." . $infoFile[$i];
						}
					}
				}
			}
		}
		$url_download = variable_get("basePathWimtv") . "videos/" . $id . "/download";
		if ($filename!=""){
			$url_download .= "?filename=" . $filename . "&ext=" . $ext;
		}
		try {
			$ch = curl_init();
			curl_setopt($ch, CURLOPT_URL,  $url_download);

			curl_setopt($ch, CURLOPT_HEADER, 1);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
			curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1);
			curl_setopt($ch, CURLOPT_USERPWD, $credential);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

			$file = curl_exec($ch);

			$file_array = explode("\n\r", $file, 2);
			$header_array = explode("\n", $file_array[0]);
			foreach($header_array as $header_value) {
			  $header_pieces = explode(':', $header_value);
			  if(count($header_pieces) == 2) {
					$headers[$header_pieces[0]] = trim($header_pieces[1]);
			  }
			}

			header('Content-type: ' . $headers['Content-Type']);
			$explodeContent = explode("filename=",$headers['Content-Disposition']);
			$filename = $explodeContent[1];
			$checkHeader = explode(";",$headers['Content-Disposition']);
			//echo $checkHeader[1];
			$checkextension = explode(".",$checkHeader[1]);
			if ((!isset($checkextension[1]))  || ($checkextension[1]==""))
					header('Content-Disposition: ' . $headers['Content-Disposition'] . "mp4");
			else
					header('Content-Disposition: ' . $headers['Content-Disposition']);
			header('Content-Length: ' . $headers['Content-Length']);
			echo substr($file_array[1], 1);
			watchdog ("WimTvPro", "Download video: " . $filename);
			//echo "<iframe src=\"" . $url_download . "\" style=\"display:none;\" />";

		} catch (Exception $e) {
		  header('Content-type: text/plain');
		  header('Content-Disposition: attachment; filename="error.txt"');
		  echo 'Caught exception: ',  $e->getMessage(), "\n";
		  watchdog ("WimTvPro", "Il video non è stato scaricato: " . $filename);
		}
			
        break;

}