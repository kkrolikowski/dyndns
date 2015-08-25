<?php /* Smarty version Smarty-3.1.8, created on 2015-08-25 20:01:18
         compiled from "./templates/test.tpl" */ ?>
<?php /*%%SmartyHeaderCode:210027804255d980ca973289-52762173%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '2319eb374ebfda6635cffe78ca7a13eed09695fd' => 
    array (
      0 => './templates/test.tpl',
      1 => 1440525671,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '210027804255d980ca973289-52762173',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.8',
  'unifunc' => 'content_55d980cb19e131_66120171',
  'variables' => 
  array (
    'title' => 0,
    'domains' => 0,
    'domain' => 0,
    'userdata' => 0,
    'subDomList' => 0,
    'history' => 0,
    'date' => 0,
    'ip' => 0,
    'allusers' => 0,
    'col' => 0,
    'val' => 0,
    'dataid' => 0,
    'sub' => 0,
    'record' => 0,
    'subd' => 0,
    'is_active' => 0,
    'subdomain' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_55d980cb19e131_66120171')) {function content_55d980cb19e131_66120171($_smarty_tpl) {?><?php if (!is_callable('smarty_modifier_regex_replace')) include '/usr/share/php/smarty3/plugins/modifier.regex_replace.php';
?><!DOCTYPE html>
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<title><?php echo $_smarty_tpl->tpl_vars['title']->value;?>
</title>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<!-- Latest compiled and minified CSS -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css">
<!-- Optional theme -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap-theme.min.css">
<link rel="stylesheet" href="lib/formvalidation/dist/css/formValidation.min.css">
<link rel="stylesheet" href ="http://cdn.jsdelivr.net/fontawesome/4.2.0/css/font-awesome.min.css">
<link rel="stylesheet" href="static/custom-style.css">
<script type="text/javascript" src="http://code.jquery.com/jquery-2.1.4.min.js"></script>
</head>
<body>
    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script>
    <!-- Include all compiled plugins (below), or include individual files as needed -->
<script type="text/javascript" src="static/bootstrap/js/bootstrap.min.js"></script>
<script type="text/javascript" src="lib/formvalidation/dist/js/formValidation.min.js"></script>
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
        <form method="post" action="" id="registerForm">
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
        <button type="submit" class="btn btn-primary" id="registerButton" data-loading-text="Sending email...">Register</button>
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
            <a class="navbar-brand" href="/"><strong>DDNS interface</strong></a>
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
			<?php if ($_smarty_tpl->tpl_vars['userdata']->value['role']=='admin'){?>
            <li role="presentation"><a href="#users"   aria-controls="users"   role="tab" data-toggle="tab">Users</a></li>
			<?php }?>
            <li role="presentation" class="dropdown">
               <a class="dropdown-toggle" data-toggle="dropdown" href="#" role="button" aria-expanded="false">Profile<span class="caret"></span></a>
               <ul class="dropdown-menu" role="menu">
                  <li role="presentation"><a href="#profile" aria-controls="profile" role="tab" data-toggle="tab">Edit Profile</a></li>
                  <li role="presentation"><a href="#chpass" aria-controls="profile" role="tab" data-toggle="tab">Change Password</a></li>
               </ul>
            </li>
            <li role="presentation" class="dropdown">
               <a class="dropdown-toggle" data-toggle="dropdown" href="#" role="button" aria-expanded="false">Domains<span class="caret"></span></a>
               <ul class="dropdown-menu" role="menu">
                  <li role="presentation"><a href="#addDomain" aria-controls="profile" role="tab" data-toggle="tab">Add domain</a></li>
                  <?php  $_smarty_tpl->tpl_vars['sub'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['sub']->_loop = false;
 $_smarty_tpl->tpl_vars['domain'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['subDomList']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['sub']->key => $_smarty_tpl->tpl_vars['sub']->value){
$_smarty_tpl->tpl_vars['sub']->_loop = true;
 $_smarty_tpl->tpl_vars['domain']->value = $_smarty_tpl->tpl_vars['sub']->key;
?>
                  <li role="presentation"><a href="#<?php echo smarty_modifier_regex_replace($_smarty_tpl->tpl_vars['domain']->value,"/\./","_");?>
" aria-controls="profile" role="tab" data-toggle="tab"><?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
</a></li>
                  <?php } ?>
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
            <div role="tabpanel" class="tab-pane fade col-xs-4" id="history">
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
			<?php if ($_smarty_tpl->tpl_vars['userdata']->value['role']=='admin'){?>
			<div role="tabpanel" class="tab-pane fade" id="users">
               <table class="table table-hover" id="allusers">
               <thead>
                  <th>ID</th><th>Full Name</th><th>Login</th><th>E-mail</th><th>Role</th><th>Active</th><th>Subdomain</th><th>Manage</th>
               </thead>
				<?php if (isset($_smarty_tpl->tpl_vars['smarty']->value['section']['record'])) unset($_smarty_tpl->tpl_vars['smarty']->value['section']['record']);
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['name'] = 'record';
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['loop'] = is_array($_loop=$_smarty_tpl->tpl_vars['allusers']->value) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['show'] = true;
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['max'] = $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['loop'];
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['step'] = 1;
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['start'] = $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['step'] > 0 ? 0 : $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['loop']-1;
if ($_smarty_tpl->tpl_vars['smarty']->value['section']['record']['show']) {
    $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['total'] = $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['loop'];
    if ($_smarty_tpl->tpl_vars['smarty']->value['section']['record']['total'] == 0)
        $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['show'] = false;
} else
    $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['total'] = 0;
if ($_smarty_tpl->tpl_vars['smarty']->value['section']['record']['show']):

            for ($_smarty_tpl->tpl_vars['smarty']->value['section']['record']['index'] = $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['start'], $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['iteration'] = 1;
                 $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['iteration'] <= $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['total'];
                 $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['index'] += $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['step'], $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['iteration']++):
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['rownum'] = $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['iteration'];
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['index_prev'] = $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['index'] - $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['step'];
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['index_next'] = $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['index'] + $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['step'];
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['first']      = ($_smarty_tpl->tpl_vars['smarty']->value['section']['record']['iteration'] == 1);
$_smarty_tpl->tpl_vars['smarty']->value['section']['record']['last']       = ($_smarty_tpl->tpl_vars['smarty']->value['section']['record']['iteration'] == $_smarty_tpl->tpl_vars['smarty']->value['section']['record']['total']);
?>
				<tr>
					<?php  $_smarty_tpl->tpl_vars['val'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['val']->_loop = false;
 $_smarty_tpl->tpl_vars['col'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['allusers']->value[$_smarty_tpl->getVariable('smarty')->value['section']['record']['index']]; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['val']->key => $_smarty_tpl->tpl_vars['val']->value){
$_smarty_tpl->tpl_vars['val']->_loop = true;
 $_smarty_tpl->tpl_vars['col']->value = $_smarty_tpl->tpl_vars['val']->key;
?>
					<?php if ($_smarty_tpl->tpl_vars['col']->value=='id'){?>
						<?php $_smarty_tpl->tpl_vars["dataid"] = new Smarty_variable(($_smarty_tpl->tpl_vars['val']->value), null, 0);?>
					<?php }?>
					<td><?php echo $_smarty_tpl->tpl_vars['val']->value;?>
</td>
					<?php } ?>
					<td>
					<ul role="tablist" class="nav">
						<li role="presentation" class="dropdown">
							<button class="btn btn-primary btn-sm" data-toggle="dropdown" href="#" role="button" aria-expanded="false">Action<span class="caret"></span></button>
						   <ul class="dropdown-menu" role="menu">
							  <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="<?php echo $_smarty_tpl->tpl_vars['dataid']->value;?>
" class="editopt">Edit</a></li>
                <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="<?php echo $_smarty_tpl->tpl_vars['dataid']->value;?>
" class="activate">Activate</a></li>
                <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="<?php echo $_smarty_tpl->tpl_vars['dataid']->value;?>
" class="newpass">Reset password</a></li>
							  <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="<?php echo $_smarty_tpl->tpl_vars['dataid']->value;?>
" class="rmuser">Remove</a></li>
						   </ul>
						</li>
					</ul>
					</td>
				</tr>
				<?php endfor; endif; ?>
               </table>
            </div>
				<form id="UserDel" method="POST" class="form-horizontal" style="display: none;">
					<input type="hidden" name="rmu" val="user">
					<dl class="dl-horizontal bg-danger">
						<dt>ID</dt><dd title="id"></dd>
						<dt>Full name</dt><dd title="name"></dd>
						<dt>Login</dt><dd title="login"></dd>
						<dt>E-mail</dt><dd title="email"></dd>
						<dt>Subdomain</dt><dd title="subdomain"></dd>
					</dl>
				<div class="modal-footer">
					<button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>
					<button type="submit" class="btn btn-danger" id="rmUserButton">Remove</button>
				</div>
			</form>
      <form id="editUserForm" method="post" class="form-horizontal" style="display: none;">
        <div class="form-group">
          <label class="col-sm-2 control-label">ID</label>
          <div class="col-sm-2">
            <input type="text" class="form-control" name="id" disabled="disabled" />
          </div>
        </div>
        <div class="form-group">
          <label class="col-sm-2 control-label">Full name</label>
          <div class="col-sm-10">
            <input type="text" class="form-control" name="name" />
          </div>
        </div>
        <div class="form-group">
          <label class="col-sm-2 control-label">Login</label>
          <div class="col-sm-10">
            <input type="text" class="form-control" name="login" />
          </div>
        </div>
        <div class="form-group">
          <label class="col-sm-2 control-label">E-mail</label>
          <div class="col-sm-10">
            <input type="text" class="form-control" name="email" />
          </div>
        </div>
        <div class="form-group">
          <label class="col-sm-2 control-label">Active</label>
          <div class="col-sm-2">
            <select name="active" class="form-control">
                <option value="0">0</option>
                <option value="1">1</option>
            </select>
          </div>
        </div>
        <div class="form-group">
          <label class="col-sm-2 control-label">Role</label>
          <div class="col-sm-3">
            <select name="role" class="form-control">
                <option value="user">user</option>
                <option value="admin">admin</option>
            </select>
          </div>
        </div>
        <div class="form-group">
          <label class="col-sm-2 control-label">Subdomain</label>
          <div class="col-xs-4">
            <input type="text" class="form-control" name="subdomain" />
          </div>
          <div class="col-xs-6">
            <input type="hidden" class="form-control" name="domain" />
            <input type="text" class="form-control" name="domain" disabled="disabled"/>
          </div>
        </div>
        <div class="form-group">
          <div class="col-xs-5 col-xs-offset-2">
            <button type="submit" class="btn btn-primary saveBtn">Save</button>
          </div>
        </div>
      </form>
			<?php }?>
            <div role="tabpanel" class="tab-pane fade col-xs-4" id="chpass">
               <h3>Change password</h3>
                  <form method="post" action="" id="newPassForm">
	                  <input type="hidden" name="changepass" value="newpass">
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
            <div role="tabpanel" class="tab-pane fade col-sm-4" id="addDomain">
              <h3><img src="static/tools.png" alt="tools" class="img-rounded toolpic">Add Domain</h3>
              <p><strong>Just type domain name here and we'll do the rest.</strong></p>
              <form action="index.php" method="post" class="addDomainForm">
                <input type="hidden" name="newDomain">
                <div class="form-group">
                  <div class="col-xs-8 addDomTXT">
                    <input type="text" class="form-control" name="domain" placeholder="example.com">
                  </div>
                  <div class="col-xs-4 domFields">
                    <button type="submit" class="btn btn-primary">Add Domain</button>
                  </div>
                </div>
              </form>
            </div>
            <?php  $_smarty_tpl->tpl_vars['sub'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['sub']->_loop = false;
 $_smarty_tpl->tpl_vars['domain'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['subDomList']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['sub']->key => $_smarty_tpl->tpl_vars['sub']->value){
$_smarty_tpl->tpl_vars['sub']->_loop = true;
 $_smarty_tpl->tpl_vars['domain']->value = $_smarty_tpl->tpl_vars['sub']->key;
?>
            <div role="tabpanel" class="tab-pane fade col-sm-8" id="<?php echo smarty_modifier_regex_replace($_smarty_tpl->tpl_vars['domain']->value,"/\./","_");?>
">
              <h1><?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
</h1>
              <hr>
              <form action="index.php" type="post" class="addDomainForm">
                <input type="hidden" name="newSubdomain">
                <div class="form-group">
                  <div class="col-xs-4 addDomTXT">
                    <input type="text" class="form-control" name="domain">
                  </div>
                  <div class="col-xs-4 domFields">
                    <input type="hidden" class="form-control" name="basedomain" value="<?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
"/>
                    <input type="text" class="form-control" name="basedomain" disabled="disabled" value="<?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
">
                  </div>
                  <div class="col-xs-4 domFields">
                    <button type="submit" class="btn btn-primary">Add subdomain</button>
                  </div>
                </div>
              </form>
              <br>
              <h2>Subdomains</h2>
              <hr>
                <?php  $_smarty_tpl->tpl_vars['record'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['record']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['sub']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['record']->key => $_smarty_tpl->tpl_vars['record']->value){
$_smarty_tpl->tpl_vars['record']->_loop = true;
?>
                  <?php if ($_smarty_tpl->tpl_vars['record']->value!='@'){?>
                    <p><?php echo $_smarty_tpl->tpl_vars['record']->value;?>
.<?php echo $_smarty_tpl->tpl_vars['domain']->value;?>
</p>
                  <?php }?>
                <?php } ?>
            </div>
            <?php } ?>
			<div role="tabpanel" class="tab-pane fade col-md-8" id="profile">
				<h3>Personal information</h3>
				<form method="post" action="" id="profileUpdate" class="form-horizontal">
					<input type="hidden" name="update" value="user">
					<div class="form-group">
						<label for="name" class="control-label col-sm-2">Full name:</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="name" name="name" value="<?php echo $_smarty_tpl->tpl_vars['userdata']->value['name'];?>
">
						</div>
					</div>
					<div class="form-group">
						<label for="login" class="control-label col-sm-2">Login:</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="login" name="login" disabled="disabled" value="<?php echo $_smarty_tpl->tpl_vars['userdata']->value['login'];?>
">
						</div>
					</div>
					<div class="form-group">
						<label for="email" class="control-label col-sm-2">E-mail:</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="email" name="email" value="<?php echo $_smarty_tpl->tpl_vars['userdata']->value['email'];?>
">
						</div>
					</div>
					<div class="form-group">
						<label for="domain" class="control-label col-sm-2">Active:</label>
						<div class="col-sm-10">
              <?php  $_smarty_tpl->tpl_vars['is_active'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['is_active']->_loop = false;
 $_smarty_tpl->tpl_vars['subdomain'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['subd']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['is_active']->key => $_smarty_tpl->tpl_vars['is_active']->value){
$_smarty_tpl->tpl_vars['is_active']->_loop = true;
 $_smarty_tpl->tpl_vars['subdomain']->value = $_smarty_tpl->tpl_vars['is_active']->key;
?>
							<div class="radio">
								<label>
                  <?php if ($_smarty_tpl->tpl_vars['is_active']->value==1){?>
                  <input type="radio" name="subdomain" id="subdomain" value="<?php echo $_smarty_tpl->tpl_vars['subdomain']->value;?>
" checked>
                  <?php }else{ ?>
                  <input type="radio" name="subdomain" id="subdomain" value="<?php echo $_smarty_tpl->tpl_vars['subdomain']->value;?>
">
                  <?php }?>
                  <?php if ($_smarty_tpl->tpl_vars['subdomain']->value==''){?>
                    N/A
                  <?php }else{ ?>
                    <?php echo $_smarty_tpl->tpl_vars['subdomain']->value;?>

                  <?php }?>
								</label>
							</div>
              <?php } ?>
						</div>
					</div>
					<div class="col-sm-2">
					</div>
					<div class="col-sm-10">
						<button type="button" class="btn btn-primary" id="updateProfileBtn">Update profile</button>
					</div>
				</form>
        <div class="col-md-8">
          <h3>Client configuration</h3>
          <div class="mygrid-wrapper-div">
              <pre># Dynamic DNS client config
#
# dynamic DNS server address
# server =
server = bsdaemon.pl
#
# dynamic DNS server TCP port number
# port =
srv_port = 12345
#
# Authentication data
# login =
# pass =
login = <?php echo $_smarty_tpl->tpl_vars['userdata']->value['login'];?>

pass = changeme
#
# client domain name
# domain =
domain = <?php echo $_smarty_tpl->tpl_vars['userdata']->value['subdomain'];?>

#
# update interval in seconds
# interval =
interval = 30
#
# Path to logfile
# log =
log = /var/log/ddns-client.log
#
# PID file location
#
pid = /var/run/ddns-client.pid
            </pre>
          </div>
        </div>
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
          <ul>
            <li><a href="files/ddns-client-linux-32bit.zip">Linux (32bit)</a></li>
            <li><a href="files/ddns-client-linux-64bit.zip">Linux (64bit)</a></li>
            <li><a href="files/ddns-client-freebsd-32bit.zip">FreeBSD (32bit)</a></li>
            <li><a href="files/ddns-client-freebsd-64bit.zip">FreeBSD (64bit)</a></li>
            <li><a href="files/ddns-client-arm-32bit.zip">ARM (32bit)</a></li>
          </ul>
       </div>
      <div class="col-md-4">
          <h2>Online Resources</h2>
         <ul style="list-style-type: none; padding: 0px;">
            <li><a href="https://github.com/kkrolikowski/dyndns/wiki" target="_blank" style="text-decoration: none;"><img src="static/documentation.png" class="img-rounded" alt="Documentation" style="max-width: 20%; height: auto;"> <strong>dDNS project wiki</strong></a>
            <li><a href="http://ddns-ng.net/" target="_blank" style="text-decoration: none;"><img src="static/website.png" class="img-rounded" alt="Homepage" style="max-width: 20%; height: auto;"> <strong>dDNS project site</strong></a>
			<li><a href="#help" style="text-decoration: none;"><img src="static/information.ico" class="img-rounded" alt="Information" style="max-width: 20%; height: auto;"> <strong>How to use</strong></a>
         </ul>
	   </div>
    </div>
	<hr>
	<div class="row" id="help">
        <div class="col-md-6">
          <h2>How to use</h2>
          <p>To start using dynamic DNS service download appropriate client version. Zip archive contains ddns-client binary and ddns-client.conf configuration file.
          Put client binary somewhere in your $PATH and configuration file in /etc. Configuration file contains sample values that need to be changed. To get your username
          and password you need to register account. After completing registration form and account activation go to Profile -> Edit profile option and copy contents of
          Client Configuration section. Now you can paste it into your config file. The last thing you should do is setting password to one you've chosen while registration process.</p>
        </div>
      <?php }?>
    </div>
</body>
</html>
<?php }} ?>