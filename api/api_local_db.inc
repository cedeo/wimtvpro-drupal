<?php

/*
 * Functions to interface to local DB.
 */

function dbUpdateVideo($state, $status, $title, $urlThumbs, $urlPlay, $duration, $showtimeId, $categories, $contentId, $acquired_identifier) {
    $table = VIDEO_TABLE_NAME;
    $fields = array(
        'status' => $status,
        'title' => $title,
        'urlThumbs' => $urlThumbs,
        'urlPlay' => $urlPlay,
        'acquiredIdentifier' => $acquired_identifier,
        'duration' => $duration,
        'category' => $categories,
    );

    if ($state != null) {
        $fields['state'] = $state;
    }
    if ($showtimeId != null) {
        $fields['showtimeIdentifier'] = $showtimeId;
    }

    $result = db_update('{' . $table . '}')
            ->fields($fields)->condition("contentIdentifier", $contentId)
            ->execute();
//    $result = db_update('{' . $table . '}')
//            ->fields(array(
//                'state ' => $state,
//                'status' => $status,
//                'title' => $title,
//                'urlThumbs' => $urlThumbs,
//                'urlPlay' => $urlPlay,
//                'acquiredIdentifier' => $acquired_identifier,
//                'duration' => $duration,
//                'showtimeIdentifier' => $showtimeId,
//                'category' => $categories,
//            ))->condition("contentIdentifier", $contentId)
//            ->execute();

    return $result;
}

function dbInsertVideo($contentIdentifier, $title, $status = "OWNED") {
    $table = VIDEO_TABLE_NAME;
    $result = db_insert('{' . $table . '}')
                    ->fields(array(
                        'uid' => variable_get("userWimtv"),
                        'contentidentifier' => $contentIdentifier,
                        'mytimestamp' => time(),
                        'position' => 0,
                        'state' => '',
                        'viewVideoModule' => '3',
                        'status' => $status,
                        'acquiredIdentifier' => '',
                        'urlThumbs' => '',
                        'category' => '',
                        'title' => $title,
                        'duration' => '',
                        'showtimeIdentifier' => ''
                    ))->execute();
    return $result;
}

function dbDeleteVideo($contentIdentifier) {
    $table = VIDEO_TABLE_NAME;
    $query = db_delete('{' . $table . '}')
            ->condition("contentidentifier", $contentIdentifier)
            ->execute();
}

function dbGetUserVideosId($user, $filter = "") {
    $table = VIDEO_TABLE_NAME;
    switch ($filter) {
        case "":
            $query = "SELECT contentidentifier FROM {$table} WHERE uid='{$user}'";
            break;
        case "pending":
            $query = "SELECT contentidentifier FROM {$table} WHERE uid='{$user}' AND status LIKE '%|%'";
            break;
        default:
            $query = "SELECT contentidentifier FROM {$table} WHERE uid='{$user}'";
            break;
    }

    $result = db_query($query);
    $array_videos_drupal = $result->fetchAll();
    return $array_videos_drupal;
}

function dbGetVideosByVid($vid) {
//    $table = "wimtvpro_videos";

    $table = VIDEO_TABLE_NAME;
    $query = "SELECT * FROM {$table} WHERE vid='{$vid}'";
    $result = db_query($query);
    $array_videos_drupal = $result->fetchAll();
//    $elenco_video_drupal = array();
//    foreach ($array_videos_drupal as $record) {
//        array_push($elenco_video_drupal, $record->contentidentifier);
//    }
//    return $elenco_video_drupal;
    return $array_videos_drupal;
}

function dbExtractSpecificPlayist($idPlayList) {
    $table_name = PLAYLIST_TABLE_NAME;
    $query = "SELECT listVideo,
			name FROM {$table_name}
			WHERE id='" . $idPlayList . "'";
    $result = db_query($query);
    return $result->fetchAll();
}


function dbGetUserVideosIn($user, $listVideos, $showtime = false, $playlist = true) {
    $table = VIDEO_TABLE_NAME;
    $and_showtime = "";
    if ($showtime) {
        $and_showtime .= "AND state='showtime'";
    }
    
    $where = dbBuildVideosIn($listVideos, $playlist);
    $query = "SELECT * FROM {$table} WHERE uid='{$user}' {$and_showtime} {$where}";
    $result = db_query($query);
    return $result->fetchAll();
}

function dbBuildVideosIn($listVideos, $in = true) {
    if (count($listVideos)) {
        $where = " AND contentidentifier ";
        if (!$in)
            $where .= "NOT";
        $where .= " IN (";
        foreach ($listVideos as $index => $video) {
            $where .= "'" . $video . "'";
            if ($index < count($listVideos) - 1)
                $where .= ", ";
        }
        $where .= ")";
        return $where;
    }
    return "";
}

?>