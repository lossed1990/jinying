var UIManager = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_table = $('#ui_body');

        //加载手提袋计算界面
        wnd.loadCalculateReticuleUI = function() {
            var html_str =
                'todo:待添加手提袋计算界面';
            $m_ui_table.html(html_str);
        }

        //加载手提袋计算界面
        wnd.loadCalculateBoxUI = function() {
            var html_str =
                'todo:待添加礼盒计算界面';
            $m_ui_table.html(html_str);
        }


        //加载出数配置界面
        wnd.loadConfigAddRuleUI = function() {
            var html_str =
                '<div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                    <h2>总览 <small>出数规则</small></h2>\
                    <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                    <p class="text-muted font-13 m-b-30">\
                        功能描述：此处展示了目前所有的出数规则信息，您可通过该页面进行修改与删除。\
                    </p>\
                    <table id="datatable" class="table table-striped table-bordered">\
                        <thead>\
                        <tr>\
                            <th>范围</th>\
                            <th>出数</th>\
                            <th>操作</th>\
                        </tr>\
                        </thead>\
                        <tbody>\
                        <tr>\
                            <td>0-500</td>\
                            <td>30</td>\
                            <td><a href="#"><i class="fa fa-edit"></i>修改</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#"><i class="fa fa-trash-o"></i>删除</a></td>\
                        </tr>\
                        <tr>\
                            <td>500-1000</td>\
                            <td>40</td>\
                            <td><a href="#"><i class="fa fa-edit"></i>修改</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#"><i class="fa fa-trash-o"></i>删除</a></td>\
                        </tr>\
                        </tbody>\
                    </table>\
                    </div>\
                </div>\
            </div>\
            <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>新增 <small>出数规则</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：您可通过该页面新增出数规则。\
                        </p>\
                        <form class="form-horizontal form-label-left">\
                            <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">范围</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">~</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">数量</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input type="text" class="form-control" placeholder="">\
                            </div>\
                            </div>\
                            <div class="ln_solid">\
                            </div>\
                            <div class="form-group">\
                            <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                <button type="submit" class="btn btn-primary">新增</button>\
                                </div>\
                            </div>\
                        </form>\
                    </div>\
                </div>\
            </div>';
            $m_ui_table.html(html_str);
        }

        //印刷机配置界面 
        wnd.loadConfigPressUI = function() {
            var html_str =
                'todo:待添加印刷机配置界面';
            $m_ui_table.html(html_str);
        }

        //刀模配置界面 
        wnd.loadConfigModuleUI = function() {
            var html_str =
                'todo:待添加刀模配置界面';
            $m_ui_table.html(html_str);
        }

        //压痕费配置界面 
        wnd.loadConfigYahenfeiUI = function() {
            var html_str =
                'todo:待添加压痕费配置界面';
            $m_ui_table.html(html_str);
        }

        //配件配置界面 
        wnd.loadConfigPartsUI = function() {
            var html_str =
                'todo:待添加配件配置界面';
            $m_ui_table.html(html_str);
        }

        //用户配置界面 
        wnd.loadConfigUserUI = function() {
            var html_str =
                '<div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                <div class="x_title">\
                    <h2>总览 <small>用户</small></h2>\
                    <div class="clearfix"></div>\
                </div>\
                <div class="x_content">\
                    <p class="text-muted font-13 m-b-30">\
                    功能描述：此处展示了目前所有的用户信息，您可通过该页面进行删除操作。\
                    </p>\
                    <table id="table_user" class="table table-striped table-bordered">\
                    <thead>\
                        <tr>\
                        <th>用户名称</th>\
                        <th>用户角色</th>\
                        <th>操作</th>\
                        </tr>\
                    </thead>\
                    <tbody>\
                        <tr>\
                        <td>gxl</td>\
                        <td>管理员</td>\
                        <td><a href="#"><i class="fa fa-trash-o"></i>删除</a></td>\
                        </tr>\
                        <tr>\
                        <td>11</td>\
                        <td>员工</td>\
                        <td><a href="#"><i class="fa fa-trash-o"></i>删除</a></td>\
                        </tr>\
                    </tbody>\
                    </table>\
                </div>\
                </div>\
            </div>\
            <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>新增 <small>用户</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：您可通过该页面新增用户。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">用户名称</label>\
                            <div class="col-md-3 col-sm-3 col-xs-12">\
                                <input id="input_user_name" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">登陆密码</label>\
                            <div class="col-md-3 col-sm-3 col-xs-12">\
                                <input id="input_user_pw" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">用户角色</label>\
                            <div class="col-md-3 col-sm-3 col-xs-12">\
                                <select id="select_user_type" class="form-control">\
                                <option>员工</option>\
                                <option>管理员</option>\
                                </select>\
                            </div>\
                            </div>\
                            <div class="ln_solid"></div>\
                            <div class="form-group">\
                            <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                <button id="submit_adduser" class="btn btn-primary">新增</button>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>';
            $m_ui_table.html(html_str);
        }

        return wnd;
    }
};

var g_oUIManager = UIManager.createNew();