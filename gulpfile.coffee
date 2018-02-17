gulp = require 'gulp'
rename = require 'gulp-rename'
CodeGenerator = require './gulp-alcube-code-generator'
del = require 'del'
through = require 'through2'
concat = require 'gulp-concat'
runSequence = require 'run-sequence'

generated_physics_clc_dest = 'src/kernels/generated-code/physics'
physics_clc_dest =

gulp.task 'generate-physics-dtos-clc', ->
  gulp.src ['physics-dtos.yml']
    .pipe CodeGenerator.generateDtoClc()
    .pipe rename 'dtos.cl'
    .pipe gulp.dest generated_physics_clc_dest

gulp.task 'generate-physics-dtos-cpp', ->
  gulp.src ['physics-dtos.yml']
    .pipe CodeGenerator.generateDtoCpp()
    .pipe rename 'dtos.h'
    .pipe gulp.dest 'src/cpp/physics/opencl'

gulp.task 'generate-physics-prototypes', ->
  gulp.src ['physics-simulator.yml']
    .pipe CodeGenerator.generatePrototypes()
    .pipe rename 'prototypes.cl'
    .pipe gulp.dest generated_physics_clc_dest

gulp.task 'concat-clc', ->
  gulp.src [
    generated_physics_clc_dest + '/dtos.cl',
    generated_physics_clc_dest + '/prototypes.cl',
    'src/kernels/physics/*'
  ]
    .pipe(concat('all.cl'))
    .pipe gulp.dest 'src/kernels/generated-code'

gulp.task 'clean', (callback) ->
  del ['dist'], callback

gulp.task 'watch', ->
  gulp.watch 'physics-dtos.yml', ['generate-physics-dtos-clc' , 'generate-physics-dtos-cpp']

gulp.task 'build', runSequence(
  ['generate-physics-dtos-clc', 'generate-physics-dtos-cpp'],
  'generate-physics-prototypes',
  'concat-clc'
)

gulp.task 'default', ['build']
