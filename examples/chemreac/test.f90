program test

  use pcm_model
  use pcm_utils

  implicit none

  ! Declarations
  character(64) :: cwd
  character(32) :: inpfile = "/model/inpfile.json"
  character(16) :: datafile = "/data/true.csv"
  character(16) :: predfile = "/output/pred.csv"
  real(8), allocatable, dimension(:,:) :: data
  real(8), allocatable, dimension(:) :: inputs, outputs
  integer, dimension(2) :: row_lim = (/2,2318/)
  integer, dimension(2) :: col_lim = (/1,6/)
  integer(4) :: n
  type(model_type) :: model

  ! Get current working directory
  call getcwd(cwd)

  ! Load model from file
  call init_model(trim(cwd) // trim(inpfile), model)

  ! Read input data
  call read_csv(trim(cwd) // trim(datafile), row_lim, col_lim, dset=data)

  ! Flatten input data
  n = row_lim(2)-row_lim(1)
  allocate(inputs(size(data)))
  inputs = reshape(data, (/size(data)/))

  ! Perform inference
  allocate(outputs(n*model%out_tot_dim))
  call call_model(model, n, inputs, outputs)
  data = reshape(outputs, (/n, model%out_tot_dim/))

  ! Write outputs to file
  call write_csv(trim(cwd) // trim(predfile), n, dset=data)

  ! Delete model
  call delete_model(model)

end program test
