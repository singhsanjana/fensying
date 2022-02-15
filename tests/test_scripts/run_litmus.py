import subprocess

base = '../tests/'
test_base = base + 'litmus/'

cc_tests = {
    'iriw'      : (0,0),
    'mutex'     : (2,0),
    'hb_3'      : (1,1),
    'hb'        : (3,0),
    'mod_fence' : (2,0),
    'wrir_rmw'  : (2,0),
    'weak_fensying/hb'             : (2,0),
    'weak_fensying/mo_hb'          : (1,0),
    'weak_fensying/mo_hb_rmw'      : (1,0),
    'weak_fensying/mo_rf_hb'       : (2,0),
    'weak_fensying/mo_rf_hb_rfinv' : (2,0),
    'weak_fensying/rf_hb'          : (1,0)
}

c_tests = {
    'wrirc' : (2,0),
}

tests = cc_tests
tests.update(c_tests)

# min -d 10 and -n 3 to pass all tests
# flags =[[], ['-t 1']]
flags = [['-d 9']]

passed_tests = []
failed_tests = []

for test in tests:
    filepath = test_base + test + '.o$cc'
    if test in c_tests:
        filepath = test_base + test + '.o$c'

    process_command = ['python3', 'main.py', '-f', filepath]

    for exn_flags in flags:
        if not exn_flags == []:
            print('Testing ' + test + ' (' + ''.join(exn_flags) + ')')
            process_command += exn_flags
        else:
            print('Testing ' + test)


        process = subprocess.Popen(process_command, 
                                stdout=subprocess.PIPE, 
                                cwd = 'src/',
                                bufsize=1, universal_newlines=True)
        lines = process.stdout.readlines()

        optimal_synthesized  = tests[test][0]
        optimal_strengthened = tests[test][1]

        run_fail = True
        test_passed = True
        summary = False
        fail_msg = ''
        for line in lines:
            if 'ABORT' in line:
                if optimal_strengthened != 0 or optimal_synthesized != 0:
                    test_passed = False
                    fail_msg += ' Could not stop trace. Expected synthesized=' + str(optimal_synthesized) + ' strengthened=' + str(optimal_strengthened)
                continue

            if 'Model Checking time exceeded' in line:
                test_passed = False
                fail_msg += ' CDS TO'
                continue

            if 'Tool time exceeded' in line:
                test_passed = False
                fail_msg += ' Fensying TO'
                continue

            if 'RESULT SUMMARY' in line:
                run_fail = False
                summary = True
                continue

            if summary and 'synthesized:' in line:
                synthesized = int(line.split()[-2])
                if synthesized != optimal_synthesized:
                    fail_msg += ' synthesized mismatch: found=' + str(synthesized) + ' expected=' + str(optimal_synthesized)
                    test_passed = False
                continue

            if summary and 'strengthened:' in line:
                strengthened = int(line.split()[-2])
                if strengthened != optimal_strengthened:
                    fail_msg += ' strengthened mismatch: found=' + str(strengthened) + ' expected=' + str(optimal_strengthened)
                    test_passed = False
                continue

        if run_fail:
            failed_tests.append( (test, exn_flags, 'failed to run') )
        elif not test_passed:
            failed_tests.append( (test, exn_flags, fail_msg) )
        else:
            passed_tests.append(test)

print('\n\n-----------------------------------------------------')
print(str(len(passed_tests)) + ' tests passed')
print(str(len(failed_tests)) + ' tests failed')
print('-----------------------------------------------------\n\n')
print('Failed Tests:')
for (test, flags, msg) in failed_tests:
    print(test + ' with flag(s) ' + str(flags))
    print('\t' + msg)