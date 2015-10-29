{
  'variables': {
    'gtest_dir': 'gtest-1.7.0',
  },
  'targets': [
    {
      'target_name': 'gtest',
      'type': 'static_library',
      'direct_dependent_settings': {
        'include_dirs': [
          '<(gtest_dir)/include',
          '<(gtest_dir)',
        ],
      },
      'include_dirs': [
        '<(gtest_dir)/include',
        '<(gtest_dir)',
      ],
      'sources': [
        '<(gtest_dir)/src/gtest-death-test.cc',
        '<(gtest_dir)/src/gtest-filepath.cc',
        '<(gtest_dir)/src/gtest-internal-inl.h',
        '<(gtest_dir)/src/gtest-port.cc',
        '<(gtest_dir)/src/gtest-printers.cc',
        '<(gtest_dir)/src/gtest-test-part.cc',
        '<(gtest_dir)/src/gtest-typed-test.cc',
        '<(gtest_dir)/src/gtest.cc',
        '<(gtest_dir)/src/gtest_main.cc',
      ],
    }
  ],
}
