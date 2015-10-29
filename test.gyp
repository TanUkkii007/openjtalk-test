{
  'variables': {
    'gtest_dir': 'gtest-1.7.0',
    'openjtalk_dir': 'node_modules/node-openjtalk/deps'
  },
  'targets': [
    {
      'target_name': 'test',
      'type': 'executable',
      'dependencies': [
        'gtest.gyp:gtest',
        '<(openjtalk_dir)/hts_engine_api.gyp:hts_engine_api',
        '<(openjtalk_dir)/open_jtalk.gyp:open_jtalk_lib',
      ],
      'sources': ['test.cc'],
      'include_dirs': [
        '<(gtest_dir)/include'
      ],
      'libraries': [
        '-lpthread',
      ],
      'ccflags': [
        '-pthread',
      ],
    }
  ],
}