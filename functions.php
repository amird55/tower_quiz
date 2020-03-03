<?php

function connect_to_db(){
    include "dbparams.php";
                
    $mysqli = new mysqli($db_host, $db_user, $db_pass, $db_schema);
 
    if(mysqli_connect_errno()) {
        echo("Connect failed: ".mysqli_connect_error());
        exit();
    }

        //set connection encoding
    if (!$mysqli->set_charset("utf8")) {
        echo("Error loading character set utf8: ". $mysqli->error);
    }

    return $mysqli;
}
function close_connection_to_db($mysqli){
    mysqli_close($mysqli);        
}

function saveQuestion(){
    
}
function getCurrentQuestion($mysqli,$dev_id,$qid){
    $query  = "SELECT * FROM questions WHERE id=$qid";
    if(!$result = mysqli_query($mysqli,$query)) {
        return false;
    }
    if(mysqli_num_rows($result) > 0){
        $row = mysqli_fetch_assoc($result);
    }
    
    return $row;
    
}
function getCurrentQuestionAnswer(){
    
}
function getNextQuestion($mysqli,$dev_id,$q_list=''){
    $query  = "SELECT * FROM questions WHERE device_id=$dev_id";
    if(!empty($q_list)){
        $query .= " AND ( id NOT IN ($q_list) )";
    }
    $query .= " ORDER BY RAND() ";
    $query .= " LIMIT 0,1";
//    echo "$query <br >";
    
    if(!$result = mysqli_query($mysqli,$query)) {
        return false;
    }
    if(mysqli_num_rows($result) > 0){
        $row = mysqli_fetch_assoc($result);
        $q_list.=','.$row['id'];
        $q_list=ltrim($q_list,',');
        $pos=array(0,1,2,3);
        shuffle($pos);
        $qq = "UPDATE `games` SET  ";
        $qq .= "q_num=q_num+1".", ";
        $qq .= "curr_q_id=".$row['id'].", ";
        $qq .= "curr_ans=".$pos[0].", ";
        $qq .= "ans1_pos=".$pos[1].", ";
        $qq .= "ans2_pos=".$pos[2].", ";
        $qq .= "ans3_pos=".$pos[3].", ";
        $qq .= "curr_q_status='N'".", ";
        $qq .= "prev_q='$q_list'";
        $qq .= " WHERE device_id=$dev_id ";
//    echo "$qq <br >";
        if(!$rr = mysqli_query($mysqli,$qq)) {
            return false;
        }
    }
    
    return getGameParams($mysqli,$dev_id);
}
function setQuestionStatus(){
    
}
function finishGame($mysqli,$dev_id){
    $query = "UPDATE `games` SET `game_status` = 'F' WHERE device_id=$dev_id ";
    if(!$result = mysqli_query($mysqli,$query)) {
        return false;
    }
    return getGameParams($mysqli,$dev_id);
}
function startNewGame($mysqli,$dev_id){
    $query = "UPDATE `games` SET `game_status` = 'G' WHERE device_id=$dev_id ";
    if(!$result = mysqli_query($mysqli,$query)) {
        return false;
    }
    return getNextQuestion($mysqli,$dev_id);
}
function beginNewGame($mysqli,$dev_id){

    $query = "INSERT INTO games (`device_id`,`game_status`) VALUES ($dev_id,'W')";
    if(!$result = mysqli_query($mysqli,$query)) {
        return array();
    }
    return getGameParams($mysqli,$dev_id);
    
}

function getGameParams($mysqli,$dev_id){
    $row=array();
    $query = "SELECT * FROM games WHERE device_id=$dev_id ";
    if(!$result = mysqli_query($mysqli,$query)) {
        return false;
    }
    if(mysqli_num_rows($result) > 0){
        $row = mysqli_fetch_assoc($result);
    }
    
    return $row;
    
}
