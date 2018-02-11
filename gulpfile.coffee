gulp = require 'gulp'
rename = require 'gulp-rename'
CodeGenerator = require './gulp-alcube-code-generator'
del = require 'del'

gulp.task 'generate-kernel', ->
  gulp.src ['dtos.yml']
    .pipe CodeGenerator.generateKernel
    .pipe rename 'dtos.cl'
    .pipe gulp.dest 'dist'

gulp.task 'generate-cpp', ->
  gulp.src ['dtos.yml']
    .pipe CodeGenerator.generateCpp
    .pipe rename 'dtos.cpp'
    .pipe gulp.dest 'dist'

gulp.task 'clean', (callback) ->
  del ['dist'], callback

gulp.task 'default', ['generate-kernel', 'generate-cpp']
