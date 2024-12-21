program main

  use, intrinsic :: iso_c_binding
  use tf2_model
  use tf2_utils

  implicit none

  ! Declarations
  character(64) :: prefix = "/home/zanardi/Codes/ML/PyCOMET/pycomet/api/pycomet/examples/msd/"
  character(32) :: inpfile = "fortran/inpfile.json"
  character(16) :: datafile = "data.csv"
  character(16) :: predfile = "fortran/pred.csv"
  real(4), allocatable, dimension(:,:) :: dset
  real(4), allocatable, dimension(:) :: inputs, outputs
  integer, dimension(2) :: row_lim = (/2,102/)
  integer, dimension(2) :: col_lim = (/1,4/)
  integer(4) :: n
  type(model_type) :: model

  write(*,*) "Started!"

  ! Load model from file
  write(*,*) "> Loading model"
  call init_model(trim(prefix) // trim(inpfile), model)

  ! Read input data
  write(*,*) "> Loading input data"
  call read_csv_float(                                                        &
    trim(prefix) // trim(datafile), row_lim, col_lim, dset=dset               &
  )

  ! Flatten input data
  n = row_lim(2)-row_lim(1)
  allocate(inputs(size(dset)))
  inputs = reshape(dset, (/size(dset)/))

  ! Perform inference
  write(*,*) "> Performing inference"
  allocate(outputs(n*model%out_tot_dim))
  call call_model(model, n, inputs, outputs)
  dset = reshape(outputs, (/n, model%out_tot_dim/))

  ! Write outputs to file
  write(*,*) "> Saving predictions"
  call write_csv_float(                                                       &
    trim(prefix) // trim(predfile), n, dset=dset                              &
  )

  ! Delete model
  call delete_model(model)

  write(*,*) "Done!"

end program main
