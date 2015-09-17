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
         window.location.reload(true);
       },
       error: function(xhr) {
         bootbox.alert(xhr.getResponseHeader('X-Message'));
       }
     });
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
      if(response.subdomain === '@') {
        $('#editUserForm').find('[name="subdomain"]').prop('disabled', true).end();
      }
      else {
        $('#editUserForm').find('[name="subdomain"]').prop('disabled', false).end();
      }
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
  $('.addDomainForm').formValidation({
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
      domain: {
        err: 'tooltip',
        required: 'true',
        validators: {
          notEmpty: {
            message: 'Domain required'
          }
        }
      }
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
       bootbox.alert("Operation done! Please reload the page")
     },
     error: function(xhr) {
       bootbox.alert(xhr.getResponseHeader('X-Message'));
     }
   });
  });
  $('.editDomainForm').formValidation({
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
      domain: {
        err: 'tooltip',
        required: 'true',
        validators: {
          notEmpty: {
            message: 'Domain required'
          }
        }
      }
    }
  })
  .on('success.form.fv', function(e) {
    e.preventDefault();
    var $form = $(this),
    id = $form.find('[name="id"]').val();
    $.ajax({
      url: "/domains.php?id=" + id,
      method: 'PUT',
      data: $form.serialize()
    }).success(function(response) {
      var $link = $('a[data-id="' + response.id + '"]'),
      $tr = $link.closest('tr'),
      $cells  = $tr.find('td');
      var $subdomain = response.subdomain + "." + response.domain;
      // Update the cell data
     $cells
       .eq(1).html($subdomain).end();

       // Hide the dialog
       $form.parents('.bootbox').modal('hide');
    });
  });
  $(document).on('click', '.editdom', function() {
   var id = $(this).attr('data-id');
   $.ajax({
     url: "/domains.php?id=" + id,
     method: 'GET'
  }).success(function(response) {
    var domain = response.domain.substring(0, response.domain.length - 1);
    var domain_id = domain.replace(/\./g, '_');
    $('#form_'+ domain_id)
    .find('[name="id"]').val(response.id).end()
    .find('[name="domain"]').val(response.subdomain).end()
    .find('[name="basename"]').val(domain).end();
     bootbox
       .dialog({
         title: 'Edit Subdomain',
         message: $('#form_'+ domain_id),
         show: false
       })
       .on('shown.bs.modal', function() {
         $('#form_'+ domain_id)
           .show()
           .formValidation('resetForm');
       })
       .on('hide.bs.modal', function(e) {
         $('#form_'+ domain_id).hide().appendTo('body');
       })
       .modal('show');
     });
  });
  $(document).on('click', '.rmsubdomain', function(e) {
   e.preventDefault();
    var id = $(this).attr('data-id');
     $.ajax({
       url: "/domains.php?rm=" + id,
       method: 'GET'
     }).success(function() {
       var $link = $('a[data-id=' + id + ']'),
       $tr = $link.closest('tr');
       $tr.remove();
     });
  });
  $('.addSubdomainForm').formValidation({
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
      subdomain: {
        err: 'tooltip',
        required: 'true',
        validators: {
          notEmpty: {
            message: 'Domain required'
          }
        }
      }
    }
  })
  .on('success.form.fv', function(e) {
   e.preventDefault();
   var $form = $(this),
   fv = $form.data('formValidation');
   var subdomain = $form.find('[name="subdomain"]').val();
   $.ajax({
     url: "/domains.php?subd=" + subdomain,
     type: 'POST',
     data: $form.serialize()
   }).success(function(response) {
     var domain = response.domain;
     var domain_id = domain.replace(/\./g, '_');
     var $table = $('#subd_' + domain_id)
     $table.append('<tr><td>'+ response.id +'</td><td>'+ response.subdomain +'.'+response.domain+'</td><td><li role="presentation" class="dropdown" style="list-style-type: none;">' +
       '<button class="btn btn-primary btn-sm" data-toggle="dropdown" href="#" role="button" aria-expanded="false">Action<span class="caret"></span></button>' +
        '<ul class="dropdown-menu" role="menu">'+
         '<li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="' + response.id + '" class="editdom">Edit</a></li>' +
         '<li role="presentation"><a href="#" aria-controls="profile" role="tab" data-toggle="tab" data-id="' + response.id + '" class="rmsubdomain">Remove</a></li>' +
        '</ul></li></ul></td></tr>');
   });
  });
  $(document).on('click', '.rmdomain', function() {
    var ul = $(this).parentsUntil('div');
    var div = ul.parents('div');
    var div2 = div.siblings('.navbar-header');
    var domain = div2.find("a").text();
    $.ajax({
      url: "/domains.php?rmdomain=" + domain,
      type: 'GET',
      success: function(e) {
        bootbox.alert("Domain removed!");
      },
      error: function(xhr) {
        bootbox.alert(xhr.getResponseHeader('X-Message'));
      }
    });
  });
  $('.download').off('click');
  $(document).on('click', '.adm_editdom', function() {
    var i = 0;
    var id = $(this).attr('data-id');
    $.ajax({
     url: "/domains.php?domid=" + id,
     method: 'GET'
   }).success(function(response) {
     var form = $('#adm_editdom_form');
     var buttons ="<div class='modal-footer submit-domain-data'>" +
       "<button type='submit' class='btn btn-primary adm_updatedomain'>Update</button>" +
       "<button type='button' class='btn btn-default' data-dismiss='modal'>Close</button>" +
     "</div>";
    form
     .find('[name="id"]').val(response.id).end()
     .find('[name="owner"]').val(response.owner).end()
     .find('[name="serial"]').val(response.serial).end()
     .find('[name="ttl"]').val(response.ttl).end()
     .find('[name="masterdns"]').val(response.masterdns).end()
     .find('[name="hostmaster"]').val(response.hostmaster).end()
     .find('[name="origin"]').val(response.origin).end();
     $('.dynamic-div').remove();
     $('.submit-domain-data').remove();
     $.each(response.records, function(key, val) {
       $.each(val, function(k, v) {
         var html = "<div class='form-group dynamic-div'>" +
          "<div class='row'>" +
             "<div class='col-sm-4'>" +
               "<label class='control-label'>Record</label>" +
               "<input type='text' class='form-control' name='subid["+ v.subid +"][]rec["+ i +"]' value='"+ key +"'/>" +
             "</div>" +
             "<div class='col-sm-3'>" +
               "<label class='control-label'>Type</label>" +
               "<select name='subid["+ v.subid +"][]type["+ i +"]' class='form-control'>" +
                 "<option value='NS'>NS</option>" +
                 "<option value='MX'>MX</option>" +
                 "<option value='A'>A</option>" +
               "</select>"+
             "</div>" +
             "<div class='col-sm-5'>" +
               "<label class='control-label'>Value</label>" +
               "<input type='text' class='form-control' name='subid["+ v.subid +"][]value["+ i +"]' value='"+ v.ip +"' />" +
             "</div>" +
           "</div>" +
         "</div>";
         form.append(html);
         $('select[name="subid['+ v.subid +'][]type['+ i + ']"]').val(v.type).prop('selected', true);
         i++;
       });
     });
     form.append(buttons);
     bootbox
       .dialog({
         title: 'Domain details',
         message: $('#adm_editdom_form'),
         show: false
       })
       .on('shown.bs.modal', function() {
         $('#adm_editdom_form')
           .show()
           .formValidation('resetForm');
       })
       .on('hide.bs.modal', function(e) {
         $('#adm_editdom_form').hide().appendTo('body');
       })
       .modal('show');
     });
  });
  $('#adm_editdom_form').on('click', '.adm_updatedomain', function() {
    var form = $('#adm_editdom_form');
    var domain = form.find('[name="origin"]').val();
    $.ajax({
     url: "/domains.php?domain=" + domain,
     method: 'PUT',
     data: form.serializeArray(),
     success: function() {
       bootbox.alert("Done!");
     }
   });
  });
});
