$(document).ready(function() {
   $('.nav-tabs a').on('click', function(e) {
      e.preventDefault();
      $(this).tab('show');
   });
   $('#registerForm').formValidation({
      framework: 'bootstrap',
      icon: {
         valid: 'glyphicon glyphicon-ok',
         invalid: 'glyphicon glyphicon-remove',
         validating: 'glyphicon glyphicon-refresh'
      },
      addOns: {
         mandatoryIcon: {
            icon: 'glyphicon glyphicon-asterisk'
         }
      },
      fields: {
         name: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The name is required'
               },
            }
         },
         login: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The username is required'
               },
               stringLength: {
                  min: 4,
                  message: "Login must contain at least four characters"
               },
               remote: {
                  message: 'The username is not available',
                  url: '/validate.php',
        				  data: {
        					       type: 'login'
        				  },
                  type: 'POST',
				          delay: 2000
               }
            }
         },
         pass: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The password is required'
               },
               different: {
                  field: 'login',
                  message: 'The password and login cannot be the same'
               },
               stringLength: {
                  min: 6,
                  message: "Password must contain at least six characters"
               }
            }
         },
         conf_pass: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The password confirm is required'
               },
               identical: {
                  field: 'pass',
                  message: 'The password and its confirm must be the same'
               },
               stringLength: {
                  min: 6,
                  message: "Password must contain at least six characters"
               }
            }
         },
         email: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The email is required'
               },
               emailAddress: {
                  message: "Invalid e-mail address"
               },
			         remote: {
				            message: 'E-mail is not available',
				            url: '/validate.php',
				            data: {
					            type: 'email'
				            },
                    type: 'POST',
				              delay: 2000
               }
            }
         }
      }
   })
   .on('keyup', '[name="pass"]', function() {
      var isEmpty = $(this).val() == '';
      $('#registerForm')
      .formValidation('enableFieldValidators', 'pass', !isEmpty)
      .formValidation('enableFieldValidators', 'conf_pass', !isEmpty);

      // Revalidate the field when user start typing in the password field
      if ($(this).val().length == 1) {
         $('#registerForm').formValidation('validateField', 'pass')
         .formValidation('validateField', 'conf_pass');
      }
   })
   .on('success.form.fv', function(e) {
		e.preventDefault();
		var $form = $(e.target),
			fv = $form.data('formValidation');
		$.ajax({
			url: $form.attr('action'),
			type: 'POST',
			data: $form.serialize(),
			success: function(e) {
				bootbox.alert("Account created. Check your e-mail for further instructions.");
				// close modal
				$( '#newUser' ).modal( 'hide' ).e( 'bs.modal', null );
				// event after hidden
				$('#newUser').on('hidden', function(){
					$(this).e('modal', null);  // destroys modal
				})
			},
			error: function(xhr) {
				bootbox.alert(xhr.getResponseHeader('X-Message'));
			}
		});
   });
   $('#loginForm').formValidation({
      framework: 'bootstrap',
      icon: {
         valid: 'glyphicon glyphicon-ok',
         invalid: 'glyphicon glyphicon-remove',
         validating: 'glyphicon glyphicon-refresh'
      },
      fields: {
         login: {
            err: 'tooltip',
            validators: {
               notEmpty: {
               message: 'The username is required'
               },
            }
         },
         pass: {
            err: 'tooltip',
            validators: {
               notEmpty: {
               message: 'The password is required'
               }
            }
         }
      }
   });
	$('#newPassForm').formValidation({
      framework: 'bootstrap',
      icon: {
         valid: 'glyphicon glyphicon-ok',
         invalid: 'glyphicon glyphicon-remove',
         validating: 'glyphicon glyphicon-refresh'
      },
      addOns: {
         mandatoryIcon: {
            icon: 'glyphicon glyphicon-asterisk'
         }
      },
      fields: {
         oldpass: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The password is required'
               }
            }
         },
         newpass: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The password is required'
               },
               different: {
                  field: 'oldpass',
                  message: 'Old and new password should be different'
               },
               stringLength: {
                  min: 6,
                  message: "Password must contain at least six characters"
               }
            }
         },
         conf_pass: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The password confirm is required'
               },
               identical: {
                  field: 'pass',
                  message: 'The password and its confirm must be the same'
               },
               stringLength: {
                  min: 6,
                  message: "Password must contain at least six characters"
               }
            }
         }
      }
   })
   .on('keyup', '[name="newpass"]', function() {
      var isEmpty = $(this).val() == '';
      $('#newPassForm')
      .formValidation('enableFieldValidators', 'newpass', !isEmpty)
      .formValidation('enableFieldValidators', 'conf_pass', !isEmpty);

      // Revalidate the field when user start typing in the password field
      if ($(this).val().length == 1) {
         $('#newPassForm').formValidation('validateField', 'newpass')
         .formValidation('validateField', 'conf_pass');
      }
   })
   .on('success.form.fv', function(e) {
		e.preventDefault();
		var $form = $(e.target),
			fv = $form.data('formValidation');
		$.ajax({
			url: $form.attr('action'),
			type: 'POST',
			data: $form.serialize(),
			success: function() {
				bootbox.alert("Password changed")
			},
			error: function(xhr) {
				bootbox.alert(xhr.getResponseHeader('X-Message'));
			}
		});
   });
   $('#rmUserButton').on('click', function(e) {
		e.preventDefault();
		var $form = $('#UserDel'),
			id = $form.find('[title="id"]').text();
			$.ajax({
				url: "/userdetails.php?rm=" + id,
				method: 'GET'
			}).success(function() {
				var $link = $('a[data-id=' + id + ']'),
				$tr = $link.closest('tr');
				$tr.remove();
			});
   });
   $('.rmuser').on('click', function() {
		var id = $(this).attr('data-id');
		$.ajax({
			url: "/userdetails.php?id=" + id,
			method: 'GET'
		}).success(function(response) {
            // Populate the form fields with the data returned from server
            $('#UserDel')
				.find('[title="id"]').text(response.id).end()
                .find('[title="login"]').text(response.login).end()
                .find('[title="name"]').text(response.name).end()
                .find('[title="email"]').text(response.email).end()
				.find('[title="subdomain"]').text(response.subdomain).end();

            // Show the dialog
            bootbox
                .dialog({
                    title: 'Are you sure?',
                    message: $('#UserDel'),
                    show: false // We will show it manually later
                })
                .on('shown.bs.modal', function() {
                    $('#UserDel')
                        .show()                             // Show the login form
                        .formValidation('resetForm'); // Reset form
                })
                .on('hide.bs.modal', function(e) {
                    // Bootbox will remove the modal (including the body which contains the login form)
                    // after hiding the modal
                    // Therefor, we need to backup the form
                    $('#UserDel').hide().appendTo('body');
                })
                .modal('show');
        });
   });
   $('#profileUpdate').formValidation({
      framework: 'bootstrap',
      icon: {
         valid: 'glyphicon glyphicon-ok',
         invalid: 'glyphicon glyphicon-remove',
         validating: 'glyphicon glyphicon-refresh'
      },
      addOns: {
         mandatoryIcon: {
            icon: 'glyphicon glyphicon-asterisk'
         }
      },
      fields: {
         name: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The name is required'
               },
            }
         },
         email: {
            err: 'tooltip',
            required: true,
            validators: {
               notEmpty: {
                  message: 'The email is required'
               },
               emailAddress: {
                  message: "Invalid e-mail address"
               }
            }
         }
      }
   });
   $('#updateProfileBtn').on('click', function(e) {
		e.preventDefault();
		var $form = $('#profileUpdate'),
			fv = $form.data('formValidation');
		$.ajax({
			url: $form.attr('action'),
			type: 'POST',
			data: $form.serialize(),
			success: function(e) {
				bootbox.alert("Profile updated");
			}
		});
   });
   $('.activate').on('click', function(e) {
		e.preventDefault();
		var id = $(this).attr('data-id');
			$.ajax({
				url: "/userdetails.php?enable=" + id,
				method: 'GET'
			}).success(function() {
				var $link = $('a[data-id=' + id + ']'),
				$tr = $link.closest('tr');
				$td = $tr.find('td:nth-child(6)').text('1');
			});
   });
   $('#editUserForm').formValidation({
     framework: 'bootstrap',
     icon: {
        valid: 'glyphicon glyphicon-ok',
        invalid: 'glyphicon glyphicon-remove',
        validating: 'glyphicon glyphicon-refresh'
     },
     addOns: {
        mandatoryIcon: {
           icon: 'glyphicon glyphicon-asterisk'
        }
     },
     fields: {
       name: {
         err: 'tooltip',
         required: 'true',
         validators: {
           notEmpty: {
             message: 'Full name required'
           }
         }
       },
       login: {
          err: 'tooltip',
          required: true,
          validators: {
             notEmpty: {
                message: 'The username is required'
             },
             stringLength: {
                min: 4,
                message: "Login must contain at least four characters"
             }
          }
       },
       email: {
          err: 'tooltip',
          required: true,
          validators: {
             notEmpty: {
                message: 'The email is required'
             },
             emailAddress: {
                message: "Invalid e-mail address"
             }
          }
       }
     }
   })
   .on('success.form.fv', function(e) {
     e.preventDefault();
     var $form = $('#editUserForm'),
     id = $form.find('[name="id"]').val();
     $.ajax({
       url: "/userdetails.php?id=" + id,
       method: 'PUT',
       data: $form.serialize()
     }).success(function(response) {
       var $link = $('a[data-id="' + response.id + '"]'),
       $tr = $link.closest('tr'),
       $cells  = $tr.find('td');
       var $subdomain = response.subdomain + "." + response.domain;
       // Update the cell data
      $cells
        .eq(1).html(response.name).end()
        .eq(2).html(response.login).end()
        .eq(3).html(response.email).end()
        .eq(4).html(response.role).end()
        .eq(5).html(response.active).end()
        .eq(6).html($subdomain).end();

        // Hide the dialog
        $form.parents('.bootbox').modal('hide');

        // You can inform the user that the data is updated successfully
        // by highlighting the row or showing a message box
        bootbox.alert('The user profile is updated');
     });
   });
   $('.editopt').on('click', function() {
    var id = $(this).attr('data-id');
    $.ajax({
      url: "/userdetails.php?id=" + id,
      method: 'GET'
   }).success(function(response) {
     $('#editUserForm')
      .find('[name="id"]').val(response.id).end()
      .find('[name="name"]').val(response.name).end()
      .find('[name="login"]').val(response.login).end()
      .find('[name="email"]').val(response.email).end()
      .find('[name="active"]').val(response.active).end()
      .find('[name="role"]').val(response.role).end()
      .find('[name="subdomain"]').val(response.subdomain).end()
      .find('[name="domain"]').val(response.domain).end();
      bootbox
        .dialog({
          title: 'Edit user profile',
          message: $('#editUserForm'),
          show: false
        })
        .on('shown.bs.modal', function() {
          $('#editUserForm')
            .show()
            .formValidation('resetForm');
        })
        .on('hide.bs.modal', function(e) {
          $('#editUserForm').hide().appendTo('body');
        })
        .modal('show');
   });
 });
 $('.newpass').on('click', function() {
  var id = $(this).attr('data-id');
  $.ajax({
    url: "/userdetails.php?newpass=" + id,
    method: 'GET'
  }).success(function() {
    bootbox.alert('New password emailed to user');
  });
    });
  var subdomainValidators = {
    row: '.col-subdomain',
    validators: {
      notEmpty: {
        message: 'Subdomain required'
      }
    }
  },
  ipValidators = {
    row: '.col-ns-ip',
    validators: {
      notEmpty: {
        message: "Record cannot be empty"
      },
      regexp: {
        enabled: true,
        regexp: /\.$/,
        message: "Domain name must end with dot"
      },
      ip: {
        enabled: false,
        message: "Invalid IP Address"
      }
    }
  },
  originValidators = {
    row: '.col-origin',
    validators: {
      notEmpty: {
        message: "Domain name required"
      },
      regexp: {
        regexp: /\.$/,
        message: "Domain name must end with dot"
      }
    }
  },
  adminValidators = {
    row: '.col-admin-contact',
    validators: {
      notEmpty: {
        message: "Admin required"
      },
      regexp: {
        regexp: /\.$/,
        message: "Admin name must end with dot"
      }
    }
  },
  masterdnsValidators = {
    row: '.col-masterdns',
    validators: {
      notEmpty: {
        message: "Domain name required"
      },
      regexp: {
        regexp: /\.$/,
        message: "Domain name must end with dot"
      }
    }
  },
  fieldIndex = 0;
  $('#addDomainForm')
    .formValidation({
      framework: 'bootstrap',
      icon: {
        valid: 'glyphicon glyphicon-ok',
        invalid: 'glyphicon glyphicon-remove',
        validating: 'glyphicon glyphicon-refresh'
      },
      fields: {
        'domain': originValidators,
        'admin': adminValidators,
        'masterdns': masterdnsValidators,
        'nsrecord[0]': subdomainValidators,
        'ip_or_name[0]': ipValidators
      }
    })
    .on('input keyup', '[name="ip_or_name['+ fieldIndex + ']"]', function() {
      var t = $('#addDomainForm').find('select option:selected').val();
      if(t == "A") {
        $('#addDomainForm')
          .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', false, 'regexp')
          .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', true, 'ip')
          .formValidation('revalidateField', 'ip_or_name[' + fieldIndex + ']');
      }
      else {
        $('#addDomainForm')
          .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', true, 'regexp')
          .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', false, 'ip')
          .formValidation('revalidateField', 'ip_or_name[' + fieldIndex + ']');
      }
    })
    .on('click', '.addButton', function() {
      fieldIndex++;
      var $template = $('#addDomainTemplate'),
        $clone = $template
                    .clone()
                    .removeClass('hide')
                    .removeAttr('id')
                    .attr('field-index', fieldIndex)
                    .insertBefore($template);
      $clone
        .find('[name="nsrecord"]').attr('name', 'nsrecord[' + fieldIndex + ']').end()
        .find('[name="type"]').attr('name', 'type[' + fieldIndex + ']').end()
        .find('[name="ip_or_name"]').attr('name', 'ip_or_name[' + fieldIndex + ']').end();
      $('#addDomainForm').on('input keyup', '[name="ip_or_name['+ fieldIndex + ']"]', function() {
        var t = $('#addDomainForm').find('[name="type[' + fieldIndex + ']"] option:selected').val();
        if(t == "A") {
          $('#addDomainForm')
            .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', false, 'regexp')
            .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', true, 'ip')
            .formValidation('revalidateField', 'ip_or_name[' + fieldIndex + ']');
        }
        else {
          $('#addDomainForm')
            .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', true, 'regexp')
            .formValidation('enableFieldValidators', 'ip_or_name[' + fieldIndex + ']', false, 'ip')
            .formValidation('revalidateField', 'ip_or_name[' + fieldIndex + ']');
        }
      });
      $('#addDomainForm')
        .formValidation('addField', 'nsrecord[' + fieldIndex + ']', subdomainValidators)
        .formValidation('addField', 'ip_or_name[' + fieldIndex + ']', ipValidators);
    })
    .on('click', '.removeButton', function() {
      var $row = $(this).parents('.row'),
      index = $row.attr('field-index');

      $('#addDomainForm')
      .formValidation('removeField', $row.find('[name="nsrecord[' + index + '"]'))
      .formValidation('removeField', $row.find('[name="type[' + index + '"]'))
      .formValidation('removeField', $row.find('[name="ip_or_name[' + index + '"]'));
      $row.remove();
    })
    .on('click', '.prevcfg', function() {
      var $row = $('#addDomainForm .row'),
      ttl = $row.find('[name="ttl"]').val(),
      origin = $row.find('[name="domain"]').val(),
      admin = $row.find('[name="admin"]').val(),
      masterdns = $row.find('[name="masterdns"]').val();

      var rec_name = [];
      var rec_type = [];
      var rec_val = [];
      var index = $('#addDomainForm .ns-records').length - 1;

      for(i = 0; i < index; i++) {
        rec_name[i] = $row.find('[name="nsrecord[' + i + ']"]').val();
        rec_type[i] = $row.find('[name="type[' + i + ']"]').val();
        rec_val[i] = $row.find('[name="ip_or_name[' + i + ']"]').val();
      }

      var date = new Date();
      var y = date.getFullYear();
      var m = date.getMonth() + 1;
      var d = date.getDate();

      var ystr = y.toString();
      var mstr = m.toString();
      var dstr = d.toString();

      if(m < 10) {
        var mstr = "0" + m.toString();
      }
      if(d < 10) {
        var dstr = "0" + d.toString();
      }
      var serial = ystr + mstr + dstr + "00";
      var content = "<pre>$TTL " + ttl + "<br>" +
                "$ORIGIN " + origin + "<br><br>" +
                "@    IN SOA  " + masterdns + " " + admin + " (<br>" +
                "      " + serial + " ; serial<br>" +
                    "      1200       ; refresh<br>" +
                    "      1200       ; retry<br>" +
                    "      2419200    ; expiry<br>" +
                    "      86400      ; maximum<br>" +
                    ")<br>";
      for(i = 0; i < index; i++) {
        content += rec_name[i] + " IN  " + rec_type[i] + " " + rec_val[i] + "<br>";
      }
      content += "</pre>";
      $('.popover-title').text("zone: " + origin);
      $('.popover-content').html(content);
    });
    $('[data-toggle="popover"]').popover();
});
