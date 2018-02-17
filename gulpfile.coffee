gulp = require 'gulp'
rename = require 'gulp-rename'
CodeGenerator = require './gulp-alcube-code-generator'
del = require 'del'
through = require 'through2'
concat = require 'gulp-concat'
runSequence = require 'run-sequence'

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

gulp.task 'concat-clc', ->
  gulp.src [
    generated_clc_dest + '/dtos.cl',
    generated_clc_dest + '/prototypes.cl',
    'src/kernels/physics/common.cl',
    'src/kernels/physics/broadphase.cl',
    'src/kernels/physics/narrowphase.cl',
    'src/kernels/physics/constraintresolving.cl',
    'src/kernels/physics/fluid.cl',
    'src/kernels/physics/motion.cl',
    'src/kernels/physics/softbody.cl'
  ]
    .pipe(concat('all.cl'))
    .pipe gulp.dest generated_clc_dest

gulp.task 'clean', (callback) ->
  del ['dist'], callback

gulp.task 'watch', ->
  gulp.watch 'gpu-interface.yml', ['generate-dtos-clc' , 'generate-dtos-cpp']

gulp.task 'build', runSequence(
  ['generate-dtos-clc', 'generate-dtos-cpp'],
  'generate-function-prototypes',
  'concat-clc'
)

gulp.task 'default', ['build']
