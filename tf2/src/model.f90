module pcm_model

  use, intrinsic :: iso_c_binding

  implicit none

  private

  public :: init_model, delete_model, call_model, model_type

  type model_type
    type(c_ptr) :: object = c_null_ptr
    integer(c_int32_t) :: inp_tot_dim = 1
    integer(c_int32_t) :: out_tot_dim = 1
  end type model_type

  interface

    ! Constructor
    function c_init_model(inpfile) result(this) bind(c, name="init_model")
      import
      type(c_ptr) :: this
      character(kind=c_char), dimension(*) :: inpfile
    end function c_init_model

    ! Input/Output dimensions
    function c_get_inp_tot_dim(this) result(inp_tot_dim) bind(c, name="get_inp_tot_dim")
      import
      type(c_ptr), value :: this
      integer(c_int32_t) :: inp_tot_dim
    end function c_get_inp_tot_dim

    function c_get_out_tot_dim(this) result(out_tot_dim) bind(c, name="get_out_tot_dim")
      import
      type(c_ptr), value :: this
      integer(c_int32_t) :: out_tot_dim
    end function c_get_out_tot_dim

    ! Destructor
    subroutine c_delete_model(this) bind(c, name="delete_model")
      import
      type(c_ptr), value :: this
    end subroutine c_delete_model

    ! Evaluate
    subroutine c_call_model_float(this, nb_pts, inputs, outputs) bind(c, name="call_model_float")
      import
      type(c_ptr), value :: this
      integer(c_int32_t) :: nb_pts
      type(c_ptr), value :: inputs
      type(c_ptr), value :: outputs
    end subroutine c_call_model_float

    subroutine c_call_model_double(this, nb_pts, inputs, outputs) bind(c, name="call_model_double")
      import
      type(c_ptr), value :: this
      integer(c_int32_t) :: nb_pts
      type(c_ptr), value :: inputs
      type(c_ptr), value :: outputs
    end subroutine c_call_model_double

  end interface

  interface call_model
    module procedure :: call_model_float, call_model_double
  end interface

  contains

  subroutine init_model(inpfile, this)
    ! Declare in-out variables
    character(*), intent(in) :: inpfile
    type(model_type), intent(out) :: this
    ! Initialize TensorFlow model
    this%object = c_init_model(inpfile//c_null_char)
    this%inp_tot_dim = c_get_inp_tot_dim(this%object)
    this%out_tot_dim = c_get_out_tot_dim(this%object)
  end subroutine init_model

  subroutine delete_model(this)
    ! Declare in-out variables
    type(model_type), intent(inout) :: this
    ! Call C++ destructor
    call c_delete_model(this%object)
    ! Re-initialize the `model_type%object` to null pointer
    this%object = c_null_ptr
  end subroutine delete_model

  subroutine call_model_float(this, nb_pts, inputs, outputs)
    ! Declare in-out variables
    type(model_type), intent(in) :: this
    integer(c_int32_t), intent(in) :: nb_pts
    real(c_float), intent(in), target :: inputs(nb_pts*this%inp_tot_dim)
    real(c_float), intent(inout), target :: outputs(nb_pts*this%out_tot_dim)
    ! Get outputs with model call function
    call c_call_model_float(this%object, nb_pts, c_loc(inputs), c_loc(outputs))
  end subroutine call_model_float

  subroutine call_model_double(this, nb_pts, inputs, outputs)
    ! Declare in-out variables
    type(model_type), intent(in) :: this
    integer(c_int32_t), intent(in) :: nb_pts
    real(c_double), intent(in), target :: inputs(nb_pts*this%inp_tot_dim)
    real(c_double), intent(inout), target :: outputs(nb_pts*this%out_tot_dim)
    ! Get outputs with model call function
    call c_call_model_double(this%object, nb_pts, c_loc(inputs), c_loc(outputs))
  end subroutine call_model_double

end module pcm_model
