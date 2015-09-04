<!DOCTYPE html>
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<title>{$title}</title>
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
    <!--<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script> -->
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
				{foreach from=$domains item=domain}
					<option value="{$domain}">{$domain}</option>
				{/foreach}
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
            {if ! isset($smarty.session.userlogin)}
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
            {else}
            <form class="navbar-form navbar-right" method="post">
               <a class="btn btn-danger" role="button" href="?opt=logout">Sign out</a>
            </form>
            <p class="navbar-text navbar-right"><strong>{$userdata.name}</strong></p>
            {/if}
         </div><!--/.navbar-collapse -->
      </div>
   </nav>
   {if ! isset($smarty.session.userlogin)}
   <!-- Main jumbotron for a primary marketing message or call to action -->
   <div class="jumbotron">
      <div class="container">
         <h1 style="letter-spacing: -2px;">Dynamic DNS</h1>
         <p>Your'e welcome to register account and use dynamic domains for free</p>
         <button type="button" class="btn btn-primary btn-lg" data-toggle="modal" data-target="#newUser" data-whatever="@mdo">New user &raquo;</button>
      </div>
   </div>
   {/if}
    <div class="container">
      {if isset($smarty.session.userlogin)}
      <div role="tabpanel" style="padding-top: 54px;">
         <!-- Nav tabs -->
         <ul class="nav nav-tabs" role="tablist" id="menuTab">
            <li role="presentation"><a href="#summary" aria-controls="profile" role="tab" data-toggle="tab">Summary</a></li>
			<li role="presentation"><a href="#history" aria-controls="history" role="tab" data-toggle="tab">History</a></li>
			{if $userdata.role eq 'admin'}
            <li role="presentation"><a href="#users"   aria-controls="users"   role="tab" data-toggle="tab">Users</a></li>
			{/if}
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
                  <li role="presentation"><a href="#addDomain" aria-controls="profile" role="tab" data-toggle="tab"><strong>Add domain</strong></a></li>
                  <li role="separator" class="divider"></li>
                  {foreach from=$subDomList key=domain item=sub}
                  <li role="presentation"><a href="#{$domain|regex_replace:"/\./":"_"}" aria-controls="profile" role="tab" data-toggle="tab">{$domain}</a></li>
                  {/foreach}
               </ul>
            </li>
         </ul>

         <!-- Tab panes -->
         <div class="tab-content">
            <div role="tabpanel" class="tab-pane fade in active" id="summary">
               <dl class="dl-horizontal" style="padding-top: 20px; margin-left: -80px;">
                  <dt>Name</dt><dd>{$userdata.name}</dd>
                  <dt>Login</dt><dd>{$userdata.login}</dd>
                  <dt>IP</dt><dd>{$userdata.ip}</dd>
                  <dt>Subdomain</dt><dd>{$userdata.subdomain}</dd>
                  <dt>Last update</dt><dd>{$userdata.lastupdate}</dd>
               </dl>
            </div>
            <div role="tabpanel" class="tab-pane fade col-xs-4" id="history">
               <table class="table">
               <thead>
                  <th>Last change</th><th>IP Address</th>
               </thead>
               {foreach from=$history key=date item=ip}
                  <tr><td width="200px">{$date}</td><td>{$ip}</td></tr>
               {/foreach}
               </table>
            </div>
			{if $userdata.role eq 'admin'}
			<div role="tabpanel" class="tab-pane fade" id="users">
               <table class="table table-hover" id="allusers">
               <thead>
                  <th>ID</th><th>Full Name</th><th>Login</th><th>E-mail</th><th>Role</th><th>Active</th><th>Subdomain</th><th>Manage</th>
               </thead>
				{section name=record loop=$allusers}
				<tr>
					{foreach from=$allusers[record] key=col item=val}
					{if $col eq 'id'}
						{assign var="dataid" value="$val"}
					{/if}
					<td>{$val}</td>
					{/foreach}
					<td>
					<ul role="tablist" class="nav">
						<li role="presentation" class="dropdown">
							<button class="btn btn-primary btn-sm" data-toggle="dropdown" href="#" role="button" aria-expanded="false">Action<span class="caret"></span></button>
						   <ul class="dropdown-menu" role="menu">
							  <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="{$dataid}" class="editopt">Edit</a></li>
                <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="{$dataid}" class="activate">Activate</a></li>
                <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="{$dataid}" class="newpass">Reset password</a></li>
							  <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="{$dataid}" class="rmuser">Remove</a></li>
						   </ul>
						</li>
					</ul>
					</td>
				</tr>
				{/section}
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
			{/if}
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
            {foreach from=$subDomList key=domain item=sub}
            <div role="tabpanel" class="tab-pane fade col-sm-8" id="{$domain|regex_replace:"/\./":"_"}">
              <div class="row">
                <nav class="navbar navbar-default margin-top-20">
                  <div class="navbar-header">
                    <a class="navbar-brand">{$domain}</a>
                  </div>
                  <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                    <form class="navbar-form navbar-left addSubdomainForm" method="post" action="">
                      <div class="form-group">
                        <input type="hidden" name="domain" value="{$domain}">
                        <input type="hidden" name="addSubdomain">
                        <input type="text" class="form-control" name="subdomain" placeholder="Subdomain">
                      </div>
                      <button type="submit" class="btn btn-default">Add</button>
                    </form>
                    <ul class="nav navbar-nav navbar-right">
                      <li><button type="submit" class="btn btn-danger margin-top-7">Delete</button></li>
                    </ul>
                  </div>
                </nav>
              </div>
              <div class="row">
                <div class="col-xs-6">
                  <table class="table table-hover" id="subd_{$domain|regex_replace:"/\./":"_"}">
                    <thead>
                      <th>ID</th><th>Subdomain</th><th>Action</th>
                    </thead>
                    <tbody>
                      {foreach from=$sub item=record}
                        {if $record[0] neq '@'}
                          <tr><td>{$record[1]}</td><td>{$record[0]}.{$domain}</td>
                              <ul role="tablist" class="nav">
                            <td>
                                <li role="presentation" class="dropdown" style="list-style-type: none;">
                                  <button class="btn btn-primary btn-sm" data-toggle="dropdown" href="#" role="button" aria-expanded="false">Action<span class="caret"></span></button>
                                   <ul class="dropdown-menu" role="menu">
                                    <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="{$record[1]}" class="editdom">Edit</a></li>
                                    <li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="{$record[1]}" class="rmsubdomain">Remove</a></li>
                                   </ul>
                                </li>
                              </ul>
                            </td></tr>
                        {/if}
                      {/foreach}
                    </tbody>
                  </table>
                </div>
              </div>
              <div class="row">
                <form action="" type="post" class="editDomainForm form-horizontal" id="form_{$domain|regex_replace:"/\./":"_"}" style="display: none;">
                  <input type="hidden" name="id" value="{$record[1]}">
                  <div class="form-group">
                    <div class="col-xs-4 addDomTXT">
                      <input type="text" class="form-control" name="domain">
                    </div>
                    <div class="col-xs-4 domFields">
                      <input type="hidden" class="form-control" name="basedomain" value="{$domain}"/>
                      <input type="text" class="form-control" name="basedomain" disabled="disabled" value="{$domain}">
                    </div>
                  </div>
                  <div class="form-group">
                    <div class="col-xs-4">
                      <button type="submit" class="btn btn-primary">Edit subdomain</button>
                    </div>
                    <div class="col-xs-4 to-left-65px">
                      <button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>
                    </div>
                  </div>
                </form>
              </div>
            </div>
            {/foreach}
			<div role="tabpanel" class="tab-pane fade col-md-8" id="profile">
				<h3>Personal information</h3>
				<form method="post" action="" id="profileUpdate" class="form-horizontal">
					<input type="hidden" name="update" value="user">
					<div class="form-group">
						<label for="name" class="control-label col-sm-2">Full name:</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="name" name="name" value="{$userdata.name}">
						</div>
					</div>
					<div class="form-group">
						<label for="login" class="control-label col-sm-2">Login:</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="login" name="login" disabled="disabled" value="{$userdata.login}">
						</div>
					</div>
					<div class="form-group">
						<label for="email" class="control-label col-sm-2">E-mail:</label>
						<div class="col-sm-10">
							<input type="text" class="form-control" id="email" name="email" value="{$userdata.email}">
						</div>
					</div>
					<div class="form-group">
						<label for="domain" class="control-label col-sm-2">Active:</label>
						<div class="col-sm-10">
              {foreach from=$subd key=subdomain item=is_active}
							<div class="radio">
								<label>
                  {if $is_active eq 1}
                  <input type="radio" name="subdomain" id="subdomain" value="{$subdomain}" checked>
                  {else}
                  <input type="radio" name="subdomain" id="subdomain" value="{$subdomain}">
                  {/if}
                  {if $subdomain eq ''}
                    N/A
                  {else}
                    {$subdomain}
                  {/if}
								</label>
							</div>
              {/foreach}
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
login = {$userdata.login}
pass = changeme
#
# client domain name
# domain =
domain = {$userdata.subdomain}
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
      {else}
      <!-- Example row of columns -->
      <div class="row">
        <div class="col-md-4">
          <h2>Available Domains</h2>
          <p>Domains you can use</p>
  	   <ul>
		{foreach from=$domains item=domain}
			<li>{$domain}</li>
		{/foreach}
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
      {/if}
    </div>
</body>
</html>
