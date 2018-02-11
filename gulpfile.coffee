gulp = require 'gulp'
rename = require 'gulp-rename'
CodeGenerator = require './gulp-alcube-code-generator'
del = require 'del'
through = require 'through2'

gulp.task 'generate-kernel', ->
  gulp.src ['dtos.yml']
    .pipe CodeGenerator.generateKernel()
    .pipe rename 'dtos.cl'
    .pipe gulp.dest 'src/kernels/physics'

gulp.task 'generate-cpp', ->
  gulp.src ['dtos.yml']
    .pipe CodeGenerator.generateCpp()
    .pipe rename 'dtos.h'
    .pipe gulp.dest 'src/cpp/physics/opencl'

gulp.task 'clean', (callback) ->
  del ['dist'], callback

gulp.task 'watch', ->
  gulp.watch 'dtos.yml', ['generate-kernel' , 'generate-cpp']

gulp.task 'default', ['generate-kernel', 'generate-cpp']
