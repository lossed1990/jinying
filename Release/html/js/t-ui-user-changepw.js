var UIUserChangePW = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');

        var $m_ui_btn_submit = undefined;
        var $m_ui_input_oldpw = undefined;
        var $m_ui_input_newpw = undefined;
        var $m_ui_input_newpw2 = undefined;

        //=====公共方法=====
        wnd.load = function() {
            loadUI();
        }

        //加载用户配置界面
        function loadUI() {
            var html_str =
                '<div class="col-md-12 col-sm-12 col-xs-12">\
                    <div class="x_panel">\
                        <div class="x_title">\
                            <h2>密码 <small>修改</small></h2>\
                            <div class="clearfix"></div>\
                        </div>\
                        <div class="x_content">\
                            <p class="text-muted font-13 m-b-30">\
                                功能描述：您可通过该页面修改个人登陆密码。\
                            </p>\
                            <div class="form-horizontal form-label-left">\
                                <div class="form-group">\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">旧密码</label>\
                                    <div class="col-md-3 col-sm-3 col-xs-12">\
                                        <input id="input_oldpw" type="password" class="form-control" placeholder="">\
                                    </div>\
                                </div>\
                                <div class="form-group">\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">新密码</label>\
                                    <div class="col-md-3 col-sm-3 col-xs-12">\
                                        <input id="input_newpw" type="password" class="form-control" placeholder="">\
                                    </div>\
                                </div>\
                                <div class="form-group">\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">再次输入</label>\
                                    <div class="col-md-3 col-sm-3 col-xs-12">\
                                        <input id="input_newpw2" type="password" class="form-control" placeholder="">\
                                    </div>\
                                </div>\
                                <div class="ln_solid"></div>\
                                <div class="form-group">\
                                    <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                        <button id="submit_changepw" class="btn btn-primary">修改</button>\
                                    </div>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            bingSubmitEvent();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_btn_submit = $('#submit_changepw');
            $m_ui_input_oldpw = $('#input_oldpw');
            $m_ui_input_newpw = $('#input_newpw');
            $m_ui_input_newpw2 = $('#input_newpw2');
        }

        //绑定新增按钮事件
        function bingSubmitEvent() {
            $m_ui_btn_submit.on('click', function() {

                if (isNull($m_ui_input_oldpw.val().trim())) {
                    toastr.error('请输入旧密码！');
                    return;
                }

                if (isNull($m_ui_input_newpw.val().trim())) {
                    toastr.error('请输入新密码！');
                    return;
                }

                if (isNull($m_ui_input_newpw2.val().trim())) {
                    toastr.error('请再次输入密码！');
                    return;
                }

                if ($m_ui_input_newpw.val().trim() != $m_ui_input_newpw2.val().trim()) {
                    toastr.error('两次输入的密码不一致，请重试！');
                    return;
                }

                var requestParam = {
                    "name": $('#welcome_tip').html().trim(),
                    "oldpw": $m_ui_input_oldpw.val().trim(),
                    "newpw": $m_ui_input_newpw.val().trim()
                };

                $.ajax({
                    type: "POST",
                    url: 'user-changepw.zc',
                    contentType: "application/json;charset=utf-8",
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function(result) {
                        if (result.ok == 0) {
                            window.location = "/";
                        } else {
                            toastr.error('密码修改失败,' + result.errorinfo);
                        }
                    },
                    error: function() {
                        toastr.error('密码修改失败,请检查服务器及网络后重试！');
                    }
                });
            });
        }

        return wnd;
    }
};

var g_oUserChangePW = UIUserChangePW.createNew();