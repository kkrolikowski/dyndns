<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>{$title}</title>
<meta http-equiv="Content-Type" content="text/html; charset=uft-8" />
<link rel="stylesheet" href="../static/style.css" />
 <link href="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/css/bootstrap-combined.no-icons.min.css" rel="stylesheet">
  <script src="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/js/bootstrap.min.js"></script>
  
  <link href="runnable.css" rel="stylesheet" />
  <!-- Load jQuery and the validate plugin -->
  <script src="//code.jquery.com/jquery-1.9.1.js"></script>
  <script src="//ajax.aspnetcdn.com/ajax/jquery.validate/1.9/jquery.validate.min.js"></script>
  <script src="../static/site.js"></script>
</head>
<body>
<div id="pagewrapper">
  <div id="page">
    <div id="header">
      <h1><span class="bcol">d</span>DNS 1.0</h1>
      <h2>{$desc}</h2>
    </div>
    <div id="topnav">
      <ul>
        <li><a href="index.php">Home</a></li>
        <li><a href="?opt=download">Download</a></li>
        <li><a href="?opt=account">Account</a></li>
        <li><a href="#">Documentation</a></li>
      </ul>
    </div>
    {if $page eq 'download'}
    	<div id="main">
	      <div id="body">
	        <h2>Client software Download</h2>
	        <p>Available client versions</p>
		<ul>
		  <li>Linux <a href="#">(64-bit)</a> | <a href="files/ddns-linux_32bit-1.0.zip">(32-bit)</a></li>
		  <li>FreeBSD <a href="#">(64-bit)</a> | <a href="#">(32-bit)</a></li>
		  <li>ARM <a href="files/ddns-arm-32bit-1.0.zip">(32-bit)</a></li>
		</ul>
	      </div>
	    </div>
	{elseif $page eq 'account' || $page eq 'history' || $page eq 'users'}
	    <div id="main">
	      <div id="body">
	      {if ! isset($smarty.session.userlogin)}
	        <h2>Login page</h2>
	        <form method="post" id="loginform">
	        <input type="hidden" name="send" value="auth">
	        <div class="label">login</div><div><input type="text" name="login" id="login"></div>
	        <div class="label">password</div><div><input type="password" name="pass" id="pass"></div>
	        <div><button type="submit" class="btn btn-primary">Login</button>&nbsp;<a class="btn btn-primary" role="button" href="?opt=register">New User</a></div>
	        </form>
	         {if isset($error)}
	        	<p>{$error}</p>
	         {/if}
	      </div>
	        {else}
	       <div id="left">
	       	<ul>
			{if $userdata.role eq 'admin'}
	       		<li><a href="?opt=users">Users</a></li>
	       		<li><a href="?opt=account">Profile</a></li>
	       		<li><a href="?opt=history">History</a></li>
			{else}
	       		<li><a href="?opt=account">Profile</a></li>
	       		<li><a href="?opt=history">History</a></li>
			{/if}
	       		<li><a href="?opt=logout">Log out</a></li>
	       	</ul>
	       </div>
	       <div id="profile">
	       {if isset($history)}
			<table class="table">
			<thead>
				<th>Last change</th><th>IP Address</th>
			</thead>
			{foreach from=$history key=date item=ip}
				<tr><td width="200px">{$date}</td><td>{$ip}</td></tr>
			{/foreach}
			</table>
			</div>
		{else if isset($allusers)}
			<table class="table">
			<thead>
			<th>Login</th><th>Name</th><th>E-mail</th><th>Role</th><th>Active</th><th>Subdomain</th>
			</thead>
			{section name=record loop=$allusers}
			<tr>
			{foreach from=$allusers[record] key=col item=val}
				<td>{$val}</td>
			{/foreach}
			</tr>
			{/section}
			</table>
			</div>
	       {else}
		{if $userdata.role eq 'user'}
		<dl class="dl-horizontal">
	       	<dt>Name</dt><dd>{$userdata.name}</dd>
	       	<dt>Login</dt><dd>{$userdata.login}</dd>
	       	<dt>IP</dt><dd>{$userdata.ip}</dd>
	       	<dt>Subdomain</dt><dd>{$userdata.subdomain}</dd>
	       	<dt>Last update</dt><dd>{$userdata.lastupdate}</dd>
		</dl>
		{/if}
	       </div>
	       {/if}
		</div>
	        {/if}
	</div>
    {else if $page eq 'register'}
	{if $status eq 'confirm_user'}
	    <div id="main">
	      <div id="body">
	        <h2>Free Dynamic DNS Service</h2>
	        <p>Account created</p>
	      </div>
	    </div>
	{else}
	    <div id="main">
	      <div id="body">
	        <h2>Registration Form</h2>
	        <form method="post" id="register-form">
	        <input type="hidden" name="register" value="newuser">
	        <div class="label">Name</div><div><input type="text" name="name" id="name"></div>
	        <div class="label">Login</div><div><input type="text" name="login" id="login"></div>
	        <div class="label">Password</div><div><input type="password" name="pass" id="pass"></div>
	        <div class="label">Confirm</div><div><input type="password" name="conf_pass" id="confirmpass"></div>
	        <div class="label">Email</div><div><input type="text" name="email" id="email"></div>
	        <div class="label">Subdomain</div><div><input type="text" name="subdomain" value="example" id="subdomain"> . 
		<select name="domain">
	        	{foreach from=$domains item=domain}
	        		<option value="{$domain}">{$domain}</option>
	        	{/foreach}
		</select>
		</div>
	        <div><input type="submit" value="register">
	        </form>
	         {if isset($error)}
	        	<p>{$error}</p>
	         {/if}
	      </div>
	    </div>
	{/if}
    {else}
	    <div id="main">
	      <div id="body">
	        <h2>Free Dynamic DNS Service</h2>
	        <p>On this website you can register a free account that allows you to create a subdomain and bind it to your IP address.</p>
	        <h2>Available Domains</h2>
	        <p>Here is a list of domains that you can use for free</p>
	        <ul>
	        	{foreach from=$domains item=domain}
	        		<li>{$domain}</li>
	        	{/foreach}
	        </ul>
	      </div>
	    </div>
    {/if}
    <!-- END OF MAIN DIV TAG -->
    <div id="footer">Designed by <a href="http://www.kalyanchakravarthy.net">Kalyan Chakravarthy</a></div>
  </div>
</div>
</body>
</html>
