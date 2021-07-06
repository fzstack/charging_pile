<%!
    def pascal_case(dash):
        return ''.join([word.capitalize() for word in dash.split('_')])
%>

menu "Test Config"
    menuconfig USING_TESTS
        bool "Enable Tests"
        default n
        if USING_TESTS
            % for clazz, comps in tests.items():
            menu "${clazz | pascal_case}"
                % for comp in comps:
                config TEST_${clazz.upper()}_${comp.upper()}
                    bool "Enable ${comp | pascal_case} test"
                    default n
                %endfor
            endmenu
            %endfor
        endif
endmenu

