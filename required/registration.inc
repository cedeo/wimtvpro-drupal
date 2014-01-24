<?php
/**
  * @file
  * This file is use for Registration.
  *
  */

function wimtvproRegistration($node, $form_state) {
  $view_page = wimtvpro_alert_reg();

  if ($view_page!=""){
    //Form Registration
    $form = array();
    $form ['#attributes'] = array("enctype" => "multipart/form-data");
    $form['fieldSetPersonal'] = array (
	'#type' => 'fieldset',
	'#title' => t('Personal Information'),
	);
	$form['fieldSetPersonal']['nameWimtv'] = array(
	'#type' => 'textfield',
	'#title' => t('Name'),
	'#default_value' => !empty($node->nameWimtv) ? $node->nameWimtv : '',
	'#size' => 100,
	'#maxlength' => 200,
	'#required' => TRUE,
    );
	$form['fieldSetPersonal']['surnameWimtv'] = array(
	'#type' => 'textfield',
	'#title' => t('Surname'),
	'#default_value' => !empty($node->surnameWimtv) ? $node->surnameWimtv : '',
	'#size' => 100,
	'#maxlength' => 200,
	'#required' => TRUE,
    );
	$form['fieldSetPersonal']['emailWimtv'] = array(
	'#type' => 'textfield',
	'#title' => t('Email'),
	'#default_value' => !empty($node->emailWimtv) ? $node->emailWimtv : '',
	'#size' => 100,
	'#maxlength' => 200,
	'#required' => TRUE,
    );
	$form['fieldSetPersonal']['genderWimtv'] = array(
	'#type' => 'select',
	'#title' => t('Gender'),
	'#default_value' => !empty($node->genderWimtv) ? $node->genderWimtv : '',
	'#required' => TRUE,
	'#options' => array(
         'M' => t('M'),
         'F' => t('F'),
       ),
    );
	$form['fieldSetLogin'] = array (
	'#type' => 'fieldset',
	'#title' => t('Login Credential'),
	);
	
    $form['fieldSetLogin']['userWimtv'] = array(
	'#type' => 'textfield',
	'#title' => t('Username'),
	'#default_value' => !empty($node->userWimtv) ? $node->userWimtv : '',
	'#size' => 100,
	'#maxlength' => 200,
	'#required' => TRUE,
    );
	$form['fieldSetLogin']['passWimtv'] = array(
	'#type' => 'password',
	'#title' => t('Password'),
	'#default_value' => !empty($node->passWimtv) ? $node->passWimtv : '',
	'#size' => 100,
	'#maxlength' => 200,
	'#required' => TRUE
	);
	$form['fieldSetLogin']['reppassWimtv'] = array(
	'#type' => 'password',
	'#title' => t('Repeat Password Wimtv'),
	'#default_value' => '',
	'#size' => 100,
	'#maxlength' => 200,
	'#required' => TRUE
	);
	$form['fieldSetLogin']['terms'] = array(
	'#type' => 'checkbox',
	'#title' => t('Terms of Use'),
	'#description' => t('I have read and agree to the wim.tv') . '&reg; <a target="new" href="http://www.wim.tv/wimtv-webapp/term.do">' . t('Terms of Service') .  '</a> ' . t("and") . ' <a target="new" href="http://www.wim.tv/wimtv-webapp/privacy.do">' . t('Privacy Policies') . '</a>',
	'#default_value' => '',
	'#size' => 100,
	'#maxlength' => 200,
	'#required' => TRUE
	);
	
	/*$form['fieldSetLogin']['sandbox'] = array(
	'#type' => 'select',
	'#title' => t('Use Sandbox'),
	'#description' => t('Please select "no" to use the plugin on the WimTV server. Select "yes" to try the service only on test server'),
	'#default_value' => '',
	'#required' => TRUE,
	'#options' => array(
         'no' => t('No'),
         'yes' => t('Yes, for Developer or Test'),
       ),
	);*/
	$form['fieldSetLogin']['sandbox'] = array(
		'#type' => 'hidden',
		'#value' => t('no')
	);
	$form['submit'] = array(
	'#type' => 'submit',
	'#value' => t('Register')
	);
	$form['#validate'][] = 'wimtvproRegistration_validate';
	$form['#submit'][] = 'wimtvproRegistration_submit';
	return $form;
	
  }
  
}

function wimtvproRegistration_validate($form, &$form_state) {
  $name = $form["fieldSetPersonal"]["nameWimtv"]["#value"];
  $surname = $form["fieldSetPersonal"]["surnameWimtv"]["#value"];
  $email = $form["fieldSetPersonal"]["emailWimtv"]["#value"];
  $gender = $form["fieldSetPersonal"]["genderWimtv"]["#value"];
  $username = $form["fieldSetLogin"]["userWimtv"]["#value"];
  $password = $form["fieldSetLogin"]["passWimtv"]["#value"];
  $reppassword = $form["fieldSetLogin"]["reppassWimtv"]["#value"];
  $acceptEula = $form["fieldSetLogin"]["terms"]["#value"];
  $sandbox = $form["fieldSetLogin"]["sandbox"]["#value"];
  if ($sandbox=="no") {
    variable_get('basePathWimtv', 'https://www.wim.tv/wimtv-webapp/rest/');
  } else {
    variable_get('basePathWimtv', 'http://peer.wim.tv/wimtv-webapp/rest/');
  }
  if ($password==$reppassword) {
    $params = array("acceptEula" => $acceptEula,
                    "name" => $name,
                    "surname" => $surname,
                    "email" => $email,
                    "username" => $username,
                    "password" => $password,
                    "sex" => $gender,
                    "role" => "webtv",
                    "dateOfBirth" => "01/01/1900");

	$response = apiRegistration($params); //curl_exec($ch);
	$arrayjsonst = json_decode($response);
	if ($arrayjsonst->result=="SUCCESS") {
	  variable_set('userWimtv', $username);
	  variable_set('passWimtv', $password);
	  drupal_add_js('window.location =  "' .  url("admin/config/wimtvpro") . '"','inline');
	  drupal_set_message( t("Successfully registered") );
	  
	} else {
      $testo_errore = "";
	  foreach ($arrayjsonst->messages as $message){
	    $testo_errore .=  $message->field . " : " .  $message->message . "<br/>";         	
	  }
	  form_set_error($password, t($testo_errore));
    }
  }
  else {
    form_set_error ($password, t("Password e Repeat Password isn't the same"));
  }
}

function wimtvproRegistration_submit($form, &$form_state) {
    //Unuseful function called on form submit...it's really necessary?

}
