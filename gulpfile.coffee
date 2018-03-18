gulp = require 'gulp'
rename = require 'gulp-rename'
CodeGenerator = require './gulp-alcube-code-generator'
del = require 'del'
through = require 'through2'
concat = require 'gulp-concat'
runSequence = require 'run-sequence'
coffee = require 'gulp-coffee'

definition_file = 'gpu-interface.yml'
generated_clc_dest = 'src/kernels/generated-code'

gulp.task 'generate-dtos-clc', ->
  gulp.src [definition_file]
    .pipe CodeGenerator.generateDtoClc()
    .pipe rename 'dtos.cl'
    .pipe gulp.dest generated_clc_dest

gulp.task 'generate-dtos-cpp', ->
  gulp.src [definition_file]
    .pipe CodeGenerator.generateDtoCpp()
    .pipe rename 'dtos.h'
    .pipe gulp.dest 'src/cpp/gpu'

gulp.task 'generate-function-prototypes', ->
  gulp.src [definition_file]
    .pipe CodeGenerator.generateFunctionPrototypes()
    .pipe rename 'prototypes.cl'
    .pipe gulp.dest generated_clc_dest

gulp.task 'generate-gpu-interface-header', ->
  gulp.src [definition_file]
    .pipe CodeGenerator.generateGPUInterfaceHeader()
    .pipe rename 'GPUAccessor.h'
    .pipe gulp.dest 'src/cpp/gpu'

gulp.task 'generate-gpu-interface-cpp', ->
  gulp.src [definition_file]
    .pipe CodeGenerator.generateGPUInterfaceCpp()
    .pipe rename 'GPUAccessor.cpp'
    .pipe gulp.dest 'src/cpp/gpu'

gulp.task 'compile-kernel', ->
  gulp.src [definition_file]
    .pipe CodeGenerator.compileKernel()

gulp.task 'concat-clc', ->
  gulp.src [
    generated_clc_dest + '/dtos.cl',
    generated_clc_dest + '/prototypes.cl',
    'src/kernels/physics/common.cl',
    'src/kernels/physics/input.cl',
    'src/kernels/physics/broadphase.cl',
    'src/kernels/physics/narrowphase.cl',
    'src/kernels/physics/force-calculation/init.cl',
    'src/kernels/physics/force-calculation/penalty.cl',
    'src/kernels/physics/force-calculation/friction.cl',
    'src/kernels/physics/force-calculation/constraint.cl',
    'src/kernels/physics/force-calculation/fluid.cl',
    'src/kernels/physics/motion.cl',
    'src/kernels/drawing/particle.cl'
  ]
    .pipe(concat('all.cl'))
    .pipe gulp.dest generated_clc_dest

gulp.task 'clean', (callback) ->
  del ['dist'], callback

gulp.task 'compile-coffee', () ->
  gulp.src 'src/coffee/*.coffee'
    .pipe coffee
      bare: true
    .pipe gulp.dest 'src/js'

gulp.task 'build', -> runSequence(
  'generate-dtos-clc',
  'generate-function-prototypes',
  'concat-clc',
  'compile-kernel',
  'generate-dtos-cpp',
  'generate-gpu-interface-header',
  'generate-gpu-interface-cpp',
)

gulp.task 'watch', ->
  gulp.watch [
    'gpu-interface.yml',
    'src/kernels/physics/*.cl',
    'src/kernels/physics/force-calculation/*.cl',
    'src/kernels/drawing/*.cl'
  ], ['build']

gulp.task 'watch-coffee', ->
  gulp.watch ['src/coffee/*.coffee'], ['compile-coffee']

gulp.task 'default', ['build']
