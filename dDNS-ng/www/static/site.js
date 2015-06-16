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
                  type: 'POST'
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
   .on('#registerButton', function(e) {
      e.preventDefault();
      $.ajax({
         url: "/?opt=register",
         type: "POST",
         data: $("#registerForm").serialize(),
      }).success(function(response) {
         bootbox.alert("Account created");
         // close modal
         $( '#newUser' ).modal( 'hide' ).e( 'bs.modal', null );
         // event after hidden
         $('#newUser').on('hidden', function(){
            $(this).e('modal', null);  // destroys modal
         });
      }).error(function(xhr) {
         bootbox.alert(xhr.getResponseHeader('X-Message'));
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
   $('#chpassBtn').on('click', function() {
	   $('#newPassForm')
	   .formValidation('destroy')
	   .formValidation({
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
	   bootbox.alert("Password changed");
   })
   .on('error.form.fv', function(xhr) {
	   bootbox.alert(xhr.getResponseHeader('X-Message'));
   });
   });
});
