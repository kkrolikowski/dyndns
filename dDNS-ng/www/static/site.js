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
   
});
