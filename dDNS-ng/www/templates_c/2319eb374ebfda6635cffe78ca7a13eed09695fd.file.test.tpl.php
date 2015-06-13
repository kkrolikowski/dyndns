<?php /* Smarty version Smarty-3.1.8, created on 2015-06-13 18:52:23
         compiled from "./templates/test.tpl" */ ?>
<?php /*%%SmartyHeaderCode:983822704557c5fc7c3a8d9-55588721%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '2319eb374ebfda6635cffe78ca7a13eed09695fd' => 
    array (
      0 => './templates/test.tpl',
      1 => 1434213038,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '983822704557c5fc7c3a8d9-55588721',
  'function' => 
  array (
  ),
  'variables' => 
  array (
    'title' => 0,
    'domains' => 0,
    'domain' => 0,
    'userdata' => 0,
    'history' => 0,
    'date' => 0,
    'ip' => 0,
  ),
  'has_nocache_code' => false,
  'version' => 'Smarty-3.1.8',
  'unifunc' => 'content_557c5fc807a045_14968324',
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_557c5fc807a045_14968324')) {function content_557c5fc807a045_14968324($_smarty_tpl) {?><!DOCTYPE html>
<head>
<meta charset="iso8859-2">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<title><?php echo $_smarty_tpl->tpl_vars['title']->value;?>
</title>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<!-- Latest compiled and minified CSS -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css">
<!-- Optional theme -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap-theme.min.css">
<link rel="stylesheet" href="lib/formvalidation/dist/css/formValidation.min.css">
<link rel="stylesheet" href="static/custom-style.css">
<script type="text/javascript" src="http://code.jquery.com/jquery-2.1.4.min.js"></script>
</head>
<body>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script>
    <!-- Include all compiled plugins (below), or include individual files as needed -->
<script type="text/javascript" src="static/bootstrap/js/bootstrap.min.js"></script>
<script type="text/javascript" src="lib/formvalidation/dist/js/formValidation.min.js"></script>
<script type="text/javascript" src="lib/formvalidation/dist/js/formValidation.js"></script>
<script type="text/javascript" src="lib/formvalidation/dist/js/framework/the-framework.js"></script>
<script type="text/javascript" src="lib/formvalidation/dist/js/language/pl_PL.js"></script>
<script type="text/javascript" src="lib/formvalidation/dist/js/addons/mandatoryIcon.min.js"></script>
<script type="text/javascript" src="lib/formvalidation/dist/js/framework/bootstrap.min.js"></script>
<script type="text/javascript" src="lib/bootbox.min.js"></script>
<script type="text/javascript" src="static/site.js"></script>
<div class="modal fade" id="newUser" tabindex="-1" role="dialog" aria-labelledby="newUserLabel" aria-hidden="true">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="NewUserLabel">New account</h4>
      </div>
      <div class="modal-body">
        <form method="post" id="registerForm">
	  <input type="hidden" name="register" value="newuser">
          <div class="form-group">
            <label for="name" class="control-label">Name:</label>
            <input type="text" class="form-control" id="name" name="name">
          </div>
          <div class="form-group">
            <label for="login" class="control-label">Login:</label>
            <input type="text" class="form-control" id="login" name="login">
          </div>
          <div class="form-group">
            <label for="pass" class="control-label">Password:</label>
            <input type="password" class="form-control" id="pass" name="pass">
          </div>
          <div class="form-group">
            <label for="pass_confirm" class="control-label">Confirm:</label>
            <input type="password" class="form-control" id="pass_confirm" name="conf_pass">
          </div>
          <div class="form-group">
            <label for="email" class="control-label">Email:</label>
	    <div class="input-group">
	      <span class="input-group-addon" id="basic-addon1">@</span>
              <input type="text" class="form-control" aria-describedby="basic-addon1" placeholder="user@example.com" id="email" name="email">
            </div>
          </div>
          <div class="form-group">
  	    <div class="row">
		<div class="col-xs-8">
		    <label for="subdomain" class="control-label">Subdomain:</label>
		    <input type="text" class="form-control" id="subdomain" name="subdomain" value="example">
		</div>
	    	<div class="col-xs-4 selectContainer">
		    <label for="subdomain" class="control-label">Domain:</label>
			<select name="domain" class="form-control">
				<?php  $_smarty_tpl->tpl_vars['domain'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['domain']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['domains']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['domain']->key => $_smarty_tpl->tpl_vars['domain']->value){
$_smarty_tpl->tpl_vars['domain']->_loop = true;
?>
					<option value="<?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
"><?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
</option>
				<?php } ?>
			</select>
		</div>
	    </div>
          </div>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
        <button type="submit" class="btn btn-primary" id="registerButton" >Register</button>
      </div>
     </form>
    </div>
  </div>
</div>
   <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container">
         <div class="navbar-header">
            <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
               <span class="sr-only">Toggle navigation</span>
               <span class="icon-bar"></span>
               <span class="icon-bar"></span>
               <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand" href="/"><strong>DDNS 1.0</strong></a>
         </div>
         <div id="navbar" class="navbar-collapse collapse">
            <?php if (!isset($_SESSION['userlogin'])){?>
            <form id="loginForm" class="navbar-form navbar-right" method="post">
               <input type="hidden" name="send" value="auth">
               <div class="form-group">
                  <input type="text" placeholder="Login" class="form-control" name="login">
               </div>
               <div class="form-group">
                  <input type="password" placeholder="Password" class="form-control" name="pass">
               </div>
               <button type="submit" class="btn btn-success">Sign in</button>
            </form>
            <?php }else{ ?>
            <form class="navbar-form navbar-right" method="post">
               <a class="btn btn-danger" role="button" href="?opt=logout">Sign out</a>
            </form>
            <p class="navbar-text navbar-right"><strong><?php echo $_smarty_tpl->tpl_vars['userdata']->value['name'];?>
</strong></p>
            <?php }?>
         </div><!--/.navbar-collapse -->
      </div>
   </nav>
   <?php if (!isset($_SESSION['userlogin'])){?>
   <!-- Main jumbotron for a primary marketing message or call to action -->
   <div class="jumbotron">
      <div class="container">
         <h1 style="letter-spacing: -2px;">Dynamic DNS</h1>
         <p>Your'e welcome to register account and use dynamic domains for free</p>
         <button type="button" class="btn btn-primary btn-lg" data-toggle="modal" data-target="#newUser" data-whatever="@mdo">New user &raquo;</button>
      </div>
   </div>
   <?php }?>
    <div class="container">
      <?php if (isset($_SESSION['userlogin'])){?>
      <div role="tabpanel" style="padding-top: 54px;">
         <!-- Nav tabs -->
         <ul class="nav nav-tabs" role="tablist" id="menuTab">
            <li role="presentation"><a href="#summary" aria-controls="profile" role="tab" data-toggle="tab">Summary</a></li>
            <li role="presentation"><a href="#history" aria-controls="history" role="tab" data-toggle="tab">History</a></li>
            <li role="presentation" class="dropdown">
               <a class="dropdown-toggle" data-toggle="dropdown" href="#" role="button" aria-expanded="false">Profile<span class="caret"></span></a>
               <ul class="dropdown-menu" role="menu">
                  <li role="presentation"><a href="#profile" aria-controls="profile" role="tab" data-toggle="tab">Edit Profile</a></li>
                  <li role="presentation"><a href="#chpass" aria-controls="profile" role="tab" data-toggle="tab">Change Password</a></li>
               </ul>
            </li>
         </ul>

         <!-- Tab panes -->
         <div class="tab-content">
            <div role="tabpanel" class="tab-pane fade in active" id="summary">
               <dl class="dl-horizontal" style="padding-top: 20px; margin-left: -80px;">
                  <dt>Name</dt><dd><?php echo $_smarty_tpl->tpl_vars['userdata']->value['name'];?>
</dd>
                  <dt>Login</dt><dd><?php echo $_smarty_tpl->tpl_vars['userdata']->value['login'];?>
</dd>
                  <dt>IP</dt><dd><?php echo $_smarty_tpl->tpl_vars['userdata']->value['ip'];?>
</dd>
                  <dt>Subdomain</dt><dd><?php echo $_smarty_tpl->tpl_vars['userdata']->value['subdomain'];?>
</dd>
                  <dt>Last update</dt><dd><?php echo $_smarty_tpl->tpl_vars['userdata']->value['lastupdate'];?>
</dd>
               </dl>
            </div>
            <div role="tabpanel" class="tab-pane fade" id="history">
               <table class="table">
               <thead>
                  <th>Last change</th><th>IP Address</th>
               </thead>
               <?php  $_smarty_tpl->tpl_vars['ip'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['ip']->_loop = false;
 $_smarty_tpl->tpl_vars['date'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['history']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['ip']->key => $_smarty_tpl->tpl_vars['ip']->value){
$_smarty_tpl->tpl_vars['ip']->_loop = true;
 $_smarty_tpl->tpl_vars['date']->value = $_smarty_tpl->tpl_vars['ip']->key;
?>
                  <tr><td width="200px"><?php echo $_smarty_tpl->tpl_vars['date']->value;?>
</td><td><?php echo $_smarty_tpl->tpl_vars['ip']->value;?>
</td></tr>
               <?php } ?>
               </table>
            </div>
            <div role="tabpanel" class="tab-pane fade col-xs-4" id="chpass">
               <h3>Change password</h3>
                  <form method="post" id="newPassForm">
	                  <input type="hidden" name="chpass" value="newpass">
                      <div class="form-group">
                        <label for="pass" class="control-label">Old Password:</label>
                        <input type="password" class="form-control" id="oldpass" name="oldpass">
                      </div>
                      <div class="form-group">
                        <label for="newpass" class="control-label">New Password:</label>
                        <input type="password" class="form-control" id="newpass" name="newpass">
                      </div>
                      <div class="form-group">
                        <label for="pass_confirm" class="control-label">Confirm:</label>
                        <input type="password" class="form-control" id="pass_confirm" name="conf_pass">
                      </div>
                       <button type="submit" class="btn btn-primary" id="chpassBtn">Apply</button>
                 </form>
            </div>
         </div>
      </div>
      <?php }else{ ?>
      <!-- Example row of columns -->
      <div class="row">
        <div class="col-md-4">
          <h2>Available Domains</h2>
          <p>Domains you can use</p>
  	   <ul>
		<?php  $_smarty_tpl->tpl_vars['domain'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['domain']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['domains']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['domain']->key => $_smarty_tpl->tpl_vars['domain']->value){
$_smarty_tpl->tpl_vars['domain']->_loop = true;
?>
			<li><?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
</li>
		<?php } ?>
                </ul>
        </div>
        <div class="col-md-4">
          <h2>Download</h2>
          <p>Client software</p>
       </div>
      <div class="col-md-4">
          <h2>Online Resources</h2>
         <ul style="list-style-type: none; padding: 0px;">
            <li><a href="https://github.com/kkrolikowski/dyndns/wiki" target="_blank" style="text-decoration: none;"><img src="static/documentation.png" class="img-rounded" alt="Documentation" style="max-width: 20%; height: auto;"> <strong>dDNS project wiki</strong></a>
            <li><a href="http://ddns-ng.net/" target="_blank" style="text-decoration: none;"><img src="static/website.png" class="img-rounded" alt="Homepage" style="max-width: 20%; height: auto;"> <strong>dDNS project site</strong></a>
         </ul>
	   </div>
      <?php }?>
    </div>
</body>
</html>
<?php }} ?>